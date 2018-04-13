#include "SimplePool.h"


std::list<Entity*> SimplePool::_pool;
SimplePool::SimplePool()
{
}


SimplePool::~SimplePool()
{
}

void SimplePool::subscribe(Entity * const & obj)
{
	_pool.push_back(obj);
}

void SimplePool::deSubscribe(Entity * const & obj)
{
	obj->destroy();
	_pool.remove(obj);
}

void SimplePool::drawAll()
{
	for (auto &obj : _pool)
	{
		if (!obj->isInitialized())
			obj->Init();
		obj->update();
		obj->draw();
	}
}
