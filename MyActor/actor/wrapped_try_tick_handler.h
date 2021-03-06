#ifndef __WRAPPED_TRY_TICK_HANDLER_H
#define __WRAPPED_TRY_TICK_HANDLER_H

#include "wrapped_capture.h"

template <typename Poster, typename Handler, bool = false>
class wrapped_try_tick_handler
{
	typedef RM_CREF(Handler) handler_type;
public:
	wrapped_try_tick_handler(Poster* poster, Handler& handler)
		: _poster(poster),
		_handler(std::forward<Handler>(handler))
	{
	}

	template <typename P, typename H>
	wrapped_try_tick_handler(wrapped_try_tick_handler<P, H>&& sp)
		:_poster(sp._poster),
		_handler(std::move(sp._handler))
	{
	}

	template <typename P, typename H>
	void operator=(wrapped_try_tick_handler<P, H>&& sp)
	{
		_poster = sp._poster;
		_handler = std::move(sp._handler);
	}

	template <typename... Args>
	void operator()(Args&&... args)
	{
		_poster->try_tick(wrap_capture(_handler, std::forward<Args>(args)...));
	}

	void operator()()
	{
		_poster->try_tick(_handler);
	}

	Poster* _poster;
	handler_type _handler;
};
//////////////////////////////////////////////////////////////////////////

template <typename Poster, typename Handler>
class wrapped_try_tick_handler<Poster, Handler, true>
{
	typedef RM_CREF(Handler) handler_type;
public:
	wrapped_try_tick_handler(Poster* poster, Handler& handler)
		: _poster(poster),
		_handler(std::forward<Handler>(handler))
#if (_DEBUG || DEBUG)
		, _checkOnce(std::make_shared<std::atomic<bool>>(false))
#endif
	{
	}

	template <typename P, typename H>
	wrapped_try_tick_handler(wrapped_try_tick_handler<P, H, true>&& sd)
		:_poster(sd._poster),
		_handler(std::move(sd._handler))
#if (_DEBUG || DEBUG)
		, _checkOnce(sd._checkOnce)
#endif
	{
	}

	template <typename P, typename H>
	void operator=(wrapped_try_tick_handler<P, H, true>&& sd)
	{
		_poster = sd._poster;
		_handler = std::move(sd._handler);
#if (_DEBUG || DEBUG)
		_checkOnce = sd._checkOnce;
#endif
	}

	template <typename... Args>
	void operator()(Args&&... args)
	{
		assert(!_checkOnce->exchange(true));
		_poster->try_tick(wrap_capture(std::move(_handler), std::forward<Args>(args)...));
	}

	void operator()()
	{
		assert(!_checkOnce->exchange(true));
		_poster->try_tick(std::move(_handler));
	}

	Poster* _poster;
	handler_type _handler;
#if (_DEBUG || DEBUG)
	std::shared_ptr<std::atomic<bool> > _checkOnce;
#endif
};

#endif