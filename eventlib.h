#ifndef __EVENTLIB_H__
#define __EVENTLIB_H__

#include <functional>
#include <list>
#include <algorithm>
#include <utility>
#include <atomic>
#include <mutex>
#include <future>
#include <iostream>

template <typename... Args> class event_handler
{
public:
	//function that returns void but takes any number of arguments of any type
	typedef std::function<void(Args...)> handler_func_type;
	typedef std::function<bool(Args...)> condition_func_type;
	typedef unsigned int handler_id_type;
	condition_func_type conditionFunc;


	event_handler(const handler_func_type& _handlerFunc, const condition_func_type& _conditionFunc)
		: handlerFunc(_handlerFunc), conditionFunc(_conditionFunc)
	{
		// std::cout<<"xyz"<<handlerFunc;
		handlerId = ++handlerIdCounter;
	}

	// copy constructor
	event_handler(const event_handler& src)
		: handlerFunc(src.handlerFunc), handlerId(src.handlerId), conditionFunc(src.conditionFunc)
	{
	}

	// move constructor
	event_handler(event_handler&& src)
		: handlerFunc(std::move(src.handlerFunc)), handlerId(src.handlerId), conditionFunc(src.conditionFunc)
	{
	}

	// copy assignment operator
	event_handler& operator=(const event_handler& src)
	{
		handlerFunc = src.handlerFunc;
		handlerId = src.handlerId;
		conditionFunc = src.conditionFunc;

		return *this;
	}

	// move assignment operator
	event_handler& operator=(event_handler&& src)
	{
		std::swap(handlerFunc, src.handlerFunc);
		std::swap(conditionFunc, src.conditionFunc);
		handlerId = src.handlerId;

		return *this;
	}

	// function call operator
	void operator()(Args... params) const
	{
		if (handlerFunc)
		{
			handlerFunc(params...);
		}
	}

	bool operator==(const event_handler& other) const
	{
		return handlerId == other.handlerId;
	}

	operator bool() const
	{
		return handlerFunc;
	}

	handler_id_type id() const
	{
		return handlerId;
	}

private:
	handler_func_type handlerFunc;
	handler_id_type handlerId;
	static std::atomic_uint handlerIdCounter;
};

template <typename... Args> std::atomic_uint event_handler<Args...>::handlerIdCounter(0);

template <typename... Args> class event
{
public:
	typedef event_handler<Args...> handler_type;
	typedef std::lock_guard<std::mutex> mutex_lock_type;

	event()
	{
	}

	// copy constructor
	event(const event& src)
	{
		mutex_lock_type lock(src.handlersLocker);

		handlers = src.handlers;
	}

	// move constructor
	event(event&& src)
	{
		mutex_lock_type lock(src.handlersLocker);

		handlers = std::move(src.handlers);
	}

	// copy assignment operator
	event& operator=(const event& src)
	{
		mutex_lock_type lock(handlersLocker);
		mutex_lock_type lock2(src.handlersLocker);

		handlers = src.handlers;

		return *this;
	}

	// move assignment operator
	event& operator=(event&& src)
	{
		mutex_lock_type lock(handlersLocker);
		mutex_lock_type lock2(src.handlersLocker);

		std::swap(handlers, src.handlers);

		return *this;
	}

	typename handler_type::handler_id_type add(const handler_type& handler)
	{
		mutex_lock_type lock(handlersLocker);

		handlers.push_back(handler);

		return handler.id();
	}

	typename handler_type::handler_id_type add(const typename handler_type::handler_func_type& handler,const typename handler_type::condition_func_type& conditionFunc)
	{
		return add(handler_type(handler,conditionFunc));
	}

	bool remove(const handler_type& handler)
	{
		mutex_lock_type lock(handlersLocker);

		auto it = std::find(handlers.begin(), handlers.end(), handler);
		if (it != handlers.end())
		{
			handlers.erase(it);
			return true;
		}

		return false;
	}

	bool remove_id(const typename handler_type::handler_id_type& handlerId)
	{
		mutex_lock_type lock(handlersLocker);

		auto it = std::find_if(handlers.begin(), handlers.end(),
			[handlerId](const handler_type& handler) { return handler.id() == handlerId; });
		if (it != handlers.end())
		{
			handlers.erase(it);
			return true;
		}

		return false;
	}

	void call(Args... params) const
	{
		handler_collection_type handlersCopy = get_handlers_copy();

		call_impl(handlersCopy, params...);
	}

	std::future<void> call_async(Args... params) const
	{
		return std::async(std::launch::async, [this](Args... asyncParams) { call(asyncParams...); }, params...);
	}

	void operator()(Args... params) const
	{
		call(params...);
	}


protected:
	typedef std::list<handler_type> handler_collection_type;

	void call_impl(const handler_collection_type& handlers, Args... params) const
	{
		for (const auto& handler : handlers)
		{
			if (handler.conditionFunc(params...)){
				handler(params...);
			}
		}
	}

	handler_collection_type get_handlers_copy() const
	{
		mutex_lock_type lock(handlersLocker);

		return handlers;
	}

private:
	handler_collection_type handlers;
	mutable std::mutex handlersLocker;
};

#endif // __EVENTLIB_H__
