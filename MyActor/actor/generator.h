#ifndef __GENERATOR_H
#define __GENERATOR_H

#include "my_actor.h"
#include "async_buffer.h"

//在generator函数体内，获取当前generator对象
#define co_self __co_self
//作为generator函数体首个参数
#define co_generator generator& co_self
//在generator上下文初始化中(co_end_context_init)，设置当前不捕获外部变量
#define co_context_no_capture __no_context_capture
struct __co_context_no_capture{};

//开始定义generator函数体上下文，类似于局部变量
#define co_begin_context \
	static_assert(__COUNTER__+1 == __COUNTER__, ""); __co_context_no_capture const co_context_no_capture = __co_context_no_capture();\
	goto __restart; __restart:;\
	struct co_context_tag: public co_context_base {

#define _co_end_context(__ctx__) \
	co_self._lockThis();\
	DEBUG_OPERATION(co_self._ctx->__inside = true);}\
	struct co_context_tag* __ctx = static_cast<co_context_tag*>(co_self._ctx);\
	struct co_context_tag& __refCtx = *__ctx;\
	struct co_context_tag& __ctx__ = *__ctx;\
	int __coNext = 0;\
	size_t __coSwitchTempVal = 0;\
	bool __coSwitchFirstLoopSign = false;\
	bool __coSwitchDefaultSign = false;\
	bool __coSwitchPreSign = false;\
	int __selectStep = 0;\
	int __selectCaseStep = 0;\
	bool __selectCaseDoSign = false;\
	bool __yieldSwitch = false; {

//结束generator函数体上下文定义
#define co_end_context(__ctx__) };\
	if (!co_self._ctx){	co_self._ctx = new co_context_tag();\
	_co_end_context(__ctx__)

#define _cop(__p__) decltype(__p__)& __p__
#define _co_capture0() co_context_tag()
#define _co_capture1(p1) co_context_tag(_cop(p1))
#define _co_capture2(p1,p2) co_context_tag(_cop(p1),_cop(p2))
#define _co_capture3(p1,p2,p3) co_context_tag(_cop(p1),_cop(p2),_cop(p3))
#define _co_capture4(p1,p2,p3,p4) co_context_tag(_cop(p1),_cop(p2),_cop(p3),_cop(p4))
#define _co_capture5(p1,p2,p3,p4,p5) co_context_tag(_cop(p1),_cop(p2),_cop(p3),_cop(p4),_cop(p5))
#define _co_capture6(p1,p2,p3,p4,p5,p6) co_context_tag(_cop(p1),_cop(p2),_cop(p3),_cop(p4),_cop(p5),_cop(p6))
#define _co_capture7(p1,p2,p3,p4,p5,p6,p7) co_context_tag(_cop(p1),_cop(p2),_cop(p3),_cop(p4),_cop(p5),_cop(p6),_cop(p7))
#define _co_capture8(p1,p2,p3,p4,p5,p6,p7,p8) co_context_tag(_cop(p1),_cop(p2),_cop(p3),_cop(p4),_cop(p5),_cop(p6),_cop(p7),_cop(p8))
#define _co_capture9(p1,p2,p3,p4,p5,p6,p7,p8,p9) co_context_tag(_cop(p1),_cop(p2),_cop(p3),_cop(p4),_cop(p5),_cop(p6),_cop(p7),_cop(p8),_cop(p9))
#define _co_capture10(p1,p2,p3,p4,p5,p6,p7,p8,p9,p10) co_context_tag(_cop(p1),_cop(p2),_cop(p3),_cop(p4),_cop(p5),_cop(p6),_cop(p7),_cop(p8),_cop(p9),_cop(p10))
#define _co_capture(...) _BOND_LR__(_co_capture, MPL_ARGS_SIZE(__VA_ARGS__))(__VA_ARGS__)

//结束generator函数体上下文定义，带内部变量初始化
#define co_end_context_init(__ctx__, __capture__, ...) _co_capture __capture__:__VA_ARGS__{}};\
	if (!co_self._ctx){	co_self._ctx = new co_context_tag __capture__;\
	_co_end_context(__ctx__)

//在启用co_fork时，初始化fork出来的generator状态，默认可以不用，将直接拷贝
#define co_context_fork co_context_tag(co_context_tag& host)

//在generator结束时，做最后状态清理，可以不用
#define co_context_destroy ~co_context_tag()

#define co_no_context co_begin_context; co_end_context(__noctx)

#define co_context_space_size sizeof(co_context_tag)

#define co_end_context_alloc(__alloc__, __ctx__) };\
	if (!co_self._ctx){	co_self._ctx = new(__alloc__)co_context_tag();\
	_co_end_context(__ctx__)

#define co_end_context_alloc_init(__alloc__, __ctx__, __capture__, ...) _co_capture __capture__:__VA_ARGS__{}};\
	if (!co_self._ctx){	co_self._ctx = new(__alloc__)co_context_tag __capture__;\
	_co_end_context(__ctx__)

//开始generator的代码区域
#define co_begin }\
	if (!__ctx->_sharedSign.empty()){__ctx->_sharedSign=true; __ctx->_sharedSign.reset();}\
	if (!__ctx->__coNext) {__ctx->__coNext = (__COUNTER__+1)/2;}else if (-1==__ctx->__coNext) co_stop;\
	__coNext=__ctx->__coNext; __ctx->__coNext=0;\
	switch(__coNext) { case __COUNTER__/2:;

//结束generator的代码区域
#define co_end break;default:assert(false);}\
	goto __stop; __stop:;\
	delete __ctx; co_self._ctx = __ctx = NULL; return;

#define co_end_dealloc(__dealloc__) break;default:assert(false);}\
	goto __stop; __stop:;\
	__ctx->~co_context_tag(); __dealloc__(__ctx); co_self._ctx = __ctx = NULL; return;

#define _co_yield do{\
	if(-1==__ctx->__coNext) co_stop;\
	assert(__ctx->__inside);\
	assert(!__ctx->__awaitSign && !__ctx->__sharedAwaitSign);\
	DEBUG_OPERATION(__ctx->__inside = false);\
	__ctx->__coNext = (__COUNTER__+1)/2;\
	return; case __COUNTER__/2:;\
	}while (0)

//generator的yield原语
#define co_yield \
	if(-1==__ctx->__coNext) co_stop;\
	assert(__ctx->__inside);\
	for (__yieldSwitch = false;;__yieldSwitch = true)\
	if (__yieldSwitch) {_co_yield; break;}\
	else

//generator异步回调接口
#define co_async \
	co_self.gen_strand()->wrap(std::bind([](generator_handle& host){\
	assert(host);\
	struct co_context_base* __ctx = host->_ctx;\
	if (!__ctx) return;\
	generator* host_ = host->_revert_this(host);\
	if (__ctx->__asyncSign) { __ctx->__asyncSign = false; host_->_next(); }\
	else { __ctx->__asyncSign = true; };\
	}, std::move(co_self.async_this())))

//generator可共享异步回调接口
#define co_shared_async \
	co_self.gen_strand()->wrap(std::bind([](generator_handle& host, shared_bool& sign){\
	if (sign) return;\
	struct co_context_base* __ctx = host->_ctx;\
	if (!__ctx) return;\
	if (__ctx->__asyncSign) { __ctx->__asyncSign = false; host->_next(); }\
	else { __ctx->__asyncSign = true; };\
	}, co_self.shared_this(), co_self.shared_async_sign()))

#define co_anext \
	co_self.gen_strand()->wrap(std::bind([](generator_handle& host){\
	assert(host);\
	host->_revert_this(host)->_next();\
	}, std::move(co_self.shared_this())))

//带返回值的generator异步回调接口
#define co_async_result(...) _co_async_result(std::move(co_self.async_this()), __VA_ARGS__)
#define co_anext_result(...) _co_anext_result(std::move(co_self.shared_this()), __VA_ARGS__)
#define co_async_result_(...) _co_async_same_result(std::move(co_self.async_this()), __VA_ARGS__)
#define co_anext_result_(...) _co_anext_same_result(std::move(co_self.shared_this()), __VA_ARGS__)
//带返回值的generator可共享异步回调接口
#define co_shared_async_result(...) _co_shared_async_result(co_self.shared_this(), co_self.shared_async_sign(), __VA_ARGS__)
#define co_shared_async_result_(...) _co_shared_async_same_result(co_self.shared_this(), co_self.shared_async_sign(), __VA_ARGS__)

//挂起generator，等待调度器下次触发
#define co_tick do{\
	if(-1==__ctx->__coNext) co_stop;\
	assert(__ctx->__inside);\
	co_self.gen_strand()->next_tick(std::bind([](generator_handle& host){\
	if(!host->_ctx)return; host->_revert_this(host)->_next(); }, std::move(co_self.shared_this()))); _co_yield;}while (0)

#define _co_await do{\
	if(-1==__ctx->__coNext) co_stop;\
	assert(__ctx->__inside);\
	assert(__ctx->__awaitSign || __ctx->__sharedAwaitSign);\
	DEBUG_OPERATION(__ctx->__awaitSign = __ctx->__sharedAwaitSign = false);\
	if (__ctx->__asyncSign) __ctx->__asyncSign = false;\
	else { __ctx->__asyncSign = true; _co_yield; }}while (0)

//generator await原语，与co_async之类使用
#define co_await \
	if(-1==__ctx->__coNext) co_stop;\
	assert(__ctx->__inside);\
	for (__yieldSwitch = false;;__yieldSwitch = true)\
	if (__yieldSwitch) {_co_await; break;}\
	else

#define co_next_(__host__) do{\
	__host__->_revert_this(__host__)->_co_next();\
	}while (0)

#define co_next(__host__) do{\
	generator_handle __host = __host__;\
	__host->_revert_this(__host)->_co_next();\
	}while (0)

#define co_tick_next_(__host__) do{\
	__host__->_revert_this(__host__)->_co_tick_next();\
	}while (0)

#define co_tick_next(__host__) do{\
	generator_handle __host = __host__;\
	__host->_revert_this(__host)->_co_tick_next();\
	}while (0)

#define co_async_next_(__host__) do{\
	__host__->_revert_this(__host__)->_co_async_next();\
	}while (0)

#define co_async_next(__host__) do{\
	generator_handle __host = __host__;\
	__host->_revert_this(__host)->_co_async_next();\
	}while (0)

#define co_shared_async_next(__host__, __sign__) do{\
	__host__->_co_shared_async_next(__sign__);\
	}while (0)

#define co_call_(__handler__) do{\
	if(-1==__ctx->__coNext) co_stop;\
	assert(__ctx->__inside);\
	generator::create(co_self.gen_strand(), __handler__, co_async)->run(); _co_await;\
	}while (0)

//递归调用另一个generator，直到执行完毕后接着下一行
#define co_call(...) co_call_(std::bind(__VA_ARGS__))

//sleep，毫秒
#define co_sleep_(__timer__, __ms__) do{\
	if(-1==__ctx->__coNext) co_stop;\
	assert(__ctx->__inside);\
	(__timer__)->timeout(__ms__, std::bind([](generator_handle& host){\
	if(!host->_ctx)return; host->_revert_this(host)->_next(); }, std::move(co_self.shared_this()))); _co_yield; }while (0)

//sleep，毫秒
#define co_sleep(__ms__) do{co_self._co_sleep(__ms__); _co_yield;}while (0)

//开始运行一个generator
#define co_go(__strand__) CoGo_(__strand__)-
//fork出一个generator，新的generator将在该语句下一行开始执行
#define co_fork do{{\
	if(-1==__ctx->__coNext) co_stop;\
	assert(__ctx->__inside);\
	assert(!__ctx->__awaitSign && !__ctx->__sharedAwaitSign);\
	generator_handle newGen = co_self._begin_fork();\
	newGen->_ctx = new co_context_tag(__refCtx); \
	newGen->_lockThis();\
	newGen->_ctx->clear();\
	newGen->_ctx->__lockStop = __ctx->__lockStop;\
	newGen->_ctx->__coNext = (__COUNTER__+1)/2;\
	newGen->run();\
	}if (0) case __COUNTER__/2:break;\
	}while (0)

#define co_fork_(__new__, ...) do{{\
	if(-1==__ctx->__coNext) co_stop;\
	assert(__ctx->__inside);\
	assert(!__ctx->__awaitSign && !__ctx->__sharedAwaitSign);\
	__new__ = co_self._begin_fork(__VA_ARGS__);\
	__new__->_lockThis();\
	__new__->_ctx = new co_context_tag(__refCtx); \
	__new__->_ctx->clear();\
	__new__->_ctx->__lockStop = __ctx->__lockStop;\
	__new__->_ctx->__coNext = (__COUNTER__+1)/2;\
	__new__->run();\
	}if (0) case __COUNTER__/2:break;\
	}while (0)

#define co_fork_alloc(__alloc__) do{{\
	if(-1==__ctx->__coNext) co_stop;\
	assert(__ctx->__inside);\
	assert(!__ctx->__awaitSign && !__ctx->__sharedAwaitSign);\
	generator_handle newGen = co_self._begin_fork();\
	newGen->_lockThis();\
	newGen->_ctx = new(__alloc__)co_context_tag(__refCtx); \
	newGen->_ctx->clear();\
	newGen->_ctx->__lockStop = __ctx->__lockStop;\
	newGen->_ctx->__coNext = (__COUNTER__+1)/2;\
	newGen->run();\
	}if (0) case __COUNTER__/2:break;\
	}while (0)

#define co_fork_alloc_(__alloc__, __new__, ...) do{{\
	if(-1==__ctx->__coNext) co_stop;\
	assert(__ctx->__inside);\
	assert(!__ctx->__awaitSign && !__ctx->__sharedAwaitSign);\
	__new__ = co_self._begin_fork(__VA_ARGS__);\
	__new__->_lockThis();\
	__new__->_ctx = new(__alloc__)co_context_tag(__refCtx); \
	__new__->_ctx->clear();\
	__new__->_ctx->__lockStop = __ctx->__lockStop;\
	__new__->_ctx->__coNext = (__COUNTER__+1)/2;\
	__new__->run();\
	}if (0) case __COUNTER__/2:break;\
	}while (0)
//拷贝出一个和当前一样的generator
#define co_clone do{\
	if(-1==__ctx->__coNext) co_stop;\
	co_self._begin_fork()->run();\
	}while (0)

#define co_clone_(__new__, ...) do{\
	if(-1==__ctx->__coNext) co_stop;\
	__new__=co_self._begin_fork(__VA_ARGS__); __new__->run();\
	}while (0)
//重启当前generator，从头开始
#define co_restart do{\
	if(-1==__ctx->__coNext) co_stop;\
	delete __ctx; co_self._ctx = __ctx = NULL; goto __restart;\
	}while (0)

#define co_restart_dealloc(__dealloc__) do{\
	if(-1==__ctx->__coNext) co_stop;\
	__ctx->~co_context_tag(); __dealloc__(__ctx); co_self._ctx = __ctx = NULL; goto __restart;\
	}while (0)
//结束当前generator的运行
#define co_stop do{goto __stop;} while(0)
//锁定外部generator的stop操作
#define co_lock_stop do{\
	assert(__ctx->__inside);\
	if(-1==__ctx->__coNext) co_stop;\
	assert(__ctx->__lockStop<255);\
	__ctx->__lockStop++;\
	}while (0)
//解锁外部generator的stop操作
#define co_unlock_stop do{\
	assert(__ctx->__inside);\
	assert(__ctx->__lockStop);\
	assert(-1!=__ctx->__coNext);\
	if (0==--__ctx->__lockStop && __ctx->__readyQuit) co_stop;\
	}while (0)
//当前generator调度器
#define co_strand co_self.gen_strand()

//发送一个任务到另一个strand中执行，执行完成后接着下一行
#define co_begin_send(__strand__) {if (co_self._co_send(__strand__, [&]{
#define co_end_send }))_co_yield;}
//发送一个任务到另一个strand中执行，执行完成后接着下一行
#define co_begin_async_send(__strand__) {co_self._co_async_send(__strand__, [&]{
#define co_end_async_send });_co_yield;}

#define _case(id, p) case p:goto __co_case_##id##_##p;
#define _case_default(id) default:goto __co_case_##id##_default;
#define _switch_case1(id,p1) _case(id,p1)_case_default(id)
#define _switch_case2(id,p1,p2) _case(id,p1)_case(id,p2)_case_default(id)
#define _switch_case3(id,p1,p2,p3) _case(id,p1)_case(id,p2)_case(id,p3)_case_default(id)
#define _switch_case4(id,p1,p2,p3,p4) _case(id,p1)_case(id,p2)_case(id,p3)_case(id,p4)_case_default(id)
#define _switch_case5(id,p1,p2,p3,p4,p5) _case(id,p1)_case(id,p2)_case(id,p3)_case(id,p4)_case(id,p5)_case_default(id)
#define _switch_case6(id,p1,p2,p3,p4,p5,p6) _case(id,p1)_case(id,p2)_case(id,p3)_case(id,p4)_case(id,p5)_case(id,p6)_case_default(id)
#define _switch_case7(id,p1,p2,p3,p4,p5,p6,p7) _case(id,p1)_case(id,p2)_case(id,p3)_case(id,p4)_case(id,p5)_case(id,p6)_case(id,p7)_case_default(id)
#define _switch_case8(id,p1,p2,p3,p4,p5,p6,p7,p8) _case(id,p1)_case(id,p2)_case(id,p3)_case(id,p4)_case(id,p5)_case(id,p6)_case(id,p7)_case(id,p8)_case_default(id)
#define _switch_case9(id,p1,p2,p3,p4,p5,p6,p7,p8,p9) _case(id,p1)_case(id,p2)_case(id,p3)_case(id,p4)_case(id,p5)_case(id,p6)_case(id,p7)_case(id,p8)_case(id,p9)_case_default(id)
#define _switch_case10(id,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10) _case(id,p1)_case(id,p2)_case(id,p3)_case(id,p4)_case(id,p5)_case(id,p6)_case(id,p7)_case(id,p8)_case(id,p9)_case(id,p10)_case_default(id)
#define _switch_case(id, ...) _BOND_LR__(_switch_case, MPL_ARGS_SIZE(__VA_ARGS__))(id, __VA_ARGS__)
#define co_define_int_name(__name__, __val__) enum{__name__=__val__};
//因为generator内部无法在switch-case里面co_yield，提供该宏间接实现switch效果
#define co_begin_switch_ex(id, __val__, ...) do{switch(__val__) {_switch_case(id, __VA_ARGS__)}
#define co_switch_case_ex(id, p) __co_case_##id##_##p:
#define co_switch_default_ex(id) co_switch_case_ex(id, default)
#define co_end_switch_ex }while (0)

//因为generator内部无法在switch-case里面co_yield，提供该宏间接实现switch效果（不支持嵌套）
#define co_begin_switch(__val__) for(__coSwitchPreSign=false,__coSwitchDefaultSign=false,__coSwitchFirstLoopSign=true,__coSwitchTempVal=(size_t)__val__;\
	__coSwitchFirstLoopSign || (!__coSwitchPreSign && __coSwitchDefaultSign);__coSwitchFirstLoopSign=false){if(0){
#define co_switch_case_(__val__) __coSwitchPreSign=true;}if (__coSwitchPreSign || __coSwitchTempVal==(size_t)(__val__)){
#define co_switch_case(__val__) __coSwitchPreSign=true;}if (__coSwitchPreSign || (__coSwitchFirstLoopSign&&__coSwitchTempVal==(size_t)(__val__))){
#define co_switch_default __coSwitchPreSign=true;}if(!__coSwitchDefaultSign && !__coSwitchPreSign){__coSwitchDefaultSign=true;}else{
#define co_end_switch __coSwitchPreSign=true;}}

//push数据到channel/msg_buffer
#define co_chan_push(__chan__, __st__, ...) do{(__chan__).push(co_async_result(__st__), __VA_ARGS__); _co_await;}while (0)
#define co_chan_push_void(__chan__, __st__) do{(__chan__).push(co_async_result(__st__)); _co_await;}while (0)
#define co_chan_try_push(__chan__, __st__, ...) do{(__chan__).try_push(co_async_result(__st__), __VA_ARGS__); _co_await;}while (0)
#define co_chan_try_push_void(__chan__, __st__) do{(__chan__).try_push(co_async_result(__st__)); _co_await;}while (0)
//从channel/msg_buffer中读取数据
#define co_chan_pop(__chan__, __st__, ...) do{(__chan__).pop(co_async_result_(__st__, __VA_ARGS__)); _co_await;}while (0)
#define co_chan_pop_void(__chan__, __st__) do{(__chan__).pop(co_async_result_(__st__)); _co_await;}while (0)
#define co_chan_try_pop(__chan__, __st__, ...) do{(__chan__).try_pop(co_async_result_(__st__, __VA_ARGS__)); _co_await;}while (0)
#define co_chan_try_pop_void(__chan__, __st__) do{(__chan__).try_pop(co_async_result_(__st__)); _co_await;}while (0)
//关闭channel/msg_buffer
#define co_chan_close(__chan__) do{(__chan__).close(co_async); _co_await;}while (0)

#define co_chan_use_state co_async_state __chanState
//上一次co_chan_io的状态
#define co_chan_last_state (__refCtx.__chanState)
//push/pop数据到channel/msg_buffer
#define co_chan_io(__chan__) co_await CoChanIo_<decltype(__chan__)>(__chan__, co_chan_last_state, co_self)
#define co_chan_try_io(__chan__) co_await CoChanTryIo_<decltype(__chan__)>(__chan__, co_chan_last_state, co_self)

#define co_chan_io_last_is_ok (co_async_state::co_async_ok == co_chan_last_state)
#define co_chan_io_last_is_fail (co_async_state::co_async_fail == co_chan_last_state)
#define co_chan_io_last_is_cancel (co_async_state::co_async_cancel == co_chan_last_state)
#define co_chan_io_last_is_closed (co_async_state::co_async_closed == co_chan_last_state)
#define co_chan_io_last_is_overtime (co_async_state::co_async_overtime == co_chan_last_state)

//co_chan_io/co_chan_try_io多参数读写时打包
#define co_chan_multi(...) _co_chan_multi(__VA_ARGS__)
//co_mutex上锁
#define co_mutex_lock(__mutex__) do{(__mutex__).lock(co_async); _co_await;}while (0)
//co_mutex解锁
#define co_mutex_unlock(__mutex__) do{(__mutex__).unlock(co_async); _co_await;}while (0)

#define co_select (__refCtx.__selectSign)
#define co_select_init __selectSign(co_self)
#define co_select_msg CoSelectSign_ __selectSign
#define co_select_msg_fork __selectSign(host.__selectSign)
#define co_select_state (co_select._ntfState)
#define co_select_state_is_ok (co_async_state::co_async_ok == co_select_state)
#define co_select_state_is_fail (co_async_state::co_async_fail == co_select_state)
#define co_select_state_is_cancel (co_async_state::co_async_cancel == co_select_state)
#define co_select_state_is_closed (co_async_state::co_async_closed == co_select_state)
#define co_select_state_is_overtime (co_async_state::co_async_overtime == co_select_state)

//开始从多个channel/msg_buffer中以select方式轮流读取数据(只能与co_end_select配合)
#define co_begin_select(__label__) {\
	co_lock_stop; co_select._ntfPump.reset(); co_select._ntfSign.clear(); \
	for (__selectStep=0,co_select._selectId=-1;;){\
	if (1==__selectStep) {co_select._ntfPump.pop(co_async_result_(co_select._ntfState, co_select._selectId)); _co_await; __selectStep=1;}\
	else if (2==__selectStep) {co_select._ntfPump.close(co_async); _co_await; break;};\
	if (0) {goto __select_ ## __label__; __select_ ## __label__: __selectStep=2; co_select._selectId=-1;}\
	co_begin_switch(co_select._selectId);\
	co_switch_default; if(0){

//开始从多个channel/msg_buffer中以select方式读取一次数据(只能与co_end_select_once配合)
#define co_begin_select_once {{\
	co_lock_stop; co_select._ntfPump.reset(); co_select._ntfSign.clear(); \
	for (__selectStep=0,co_select._selectId=-1;__selectStep<=2;__selectStep++){\
	if (1==__selectStep) {co_select._ntfPump.pop(co_async_result_(co_select._ntfState, co_select._selectId)); _co_await; __selectStep=1;}\
	else if (2==__selectStep) {co_select._ntfPump.close(co_async); _co_await; __selectStep=2; co_select._selectId=-1;};\
	co_begin_switch(co_select._selectId);\
	co_switch_default; if(0){

#define _co_select_case(__check__, __chan__) __selectCaseStep=0;__selectStep=1;__selectCaseDoSign=true;if(co_select._ntfSign.empty()) break;}if(1==__selectStep) break;\
	co_switch_case(__LINE__);\
	for(__selectCaseStep=0, __selectCaseDoSign=false; __selectCaseStep<2; __selectCaseStep++)\
	if (1==__selectCaseStep) {if (0==__selectStep || __selectCaseDoSign) {(__chan__).append_pop_notify([&](co_async_state){co_select._ntfPump.push([](co_async_state){}, __LINE__);}, co_select._ntfSign[__LINE__]);}\
		else if (2==__selectStep) {(__chan__).remove_pop_notify(co_async_result_(co_select._ntfState), co_select._ntfSign[__LINE__]); _co_await;__selectCaseStep=1;__selectStep=2;}}\
	else if (1==__selectStep && (__check__)) {

#define _co_select_case_once(__check__, __chan__) __selectCaseStep=0;__selectStep=1;__selectCaseDoSign=true;if(co_select._ntfSign.empty()) break;}if(1==__selectStep) break;\
	co_switch_case(__LINE__);\
	for(__selectCaseStep=0; __selectCaseStep<2; __selectCaseStep++)\
	if (1==__selectCaseStep) {if (0==__selectStep) {(__chan__).append_pop_notify([&](co_async_state){co_select._ntfPump.push([](co_async_state){}, __LINE__);}, co_select._ntfSign[__LINE__]);}\
		else if (2==__selectStep) {(__chan__).remove_pop_notify(co_async_result_(co_select._ntfState), co_select._ntfSign[__LINE__]); _co_await;__selectCaseStep=1;__selectStep=2;}}\
	else if (1==__selectStep && (__check__)) {

//从channel/msg_buffer中检测是否有数据
#define co_select_case_once_(__chan__) _co_select_case_once(1, __chan__) co_select._ntfState = co_async_state::co_async_undefined
#define co_select_case_(__chan__) co_select_case_once_(1, __chan__)
//从channel/msg_buffer中读取数据
#define co_select_check_case(__check__, __chan__, ...) _co_select_case(__check__, __chan__) (__chan__).try_pop(co_async_result_(co_select._ntfState, __VA_ARGS__)); _co_await;
#define co_select_case(__chan__, ...) co_select_check_case(1, __chan__, __VA_ARGS__)
#define co_select_check_case_to(__check__, __chan__) _co_select_case(__check__, __chan__) co_await CoChanTryIo_<decltype(__chan__)>(__chan__, co_select_state, co_self)
#define co_select_case_to(__chan__) co_select_check_case_to(1, __chan__)
//从channel/msg_buffer中读取空数据
#define co_select_check_case_void(__check__, __chan__) _co_select_case(__check__, __chan__) (__chan__).try_pop(co_async_result_(co_select._ntfState)); _co_await;
#define co_select_case_void(__chan__) co_select_check_case_void(1, __chan__)
//从channel/msg_buffer中读取一次数据
#define co_select_check_case_once(__check__, __chan__, ...) _co_select_case_once(__check__, __chan__) (__chan__).try_pop(co_async_result_(co_select._ntfState, __VA_ARGS__)); _co_await;
#define co_select_case_once(__chan__, ...) co_select_check_case_once(1, __chan__, __VA_ARGS__)
#define co_select_check_case_once_to(__check__, __chan__) _co_select_case_once(__check__, __chan__) co_await CoChanTryIo_<decltype(__chan__)>(__chan__, co_select_state, co_self)
#define co_select_case_once_to(__chan__) co_select_check_case_once_to(1, __chan__)
//从channel/msg_buffer中读取一次空数据
#define co_select_check_case_void_once(__check__, __chan__) _co_select_case_once(__check__, __chan__) (__chan__).try_pop(co_async_result_(co_select._ntfState)); _co_await;
#define co_select_case_void_once(__chan__) co_select_check_case_void_once(1, __chan__)

//结束从多个channel/msg_buffer中以select方式读取数据(只能与co_begin_select配合)
#define co_end_select __selectCaseStep=0;__selectStep=1;__selectCaseDoSign=true;if(co_select._ntfSign.empty()) break;}\
	co_end_switch; if (0==__selectStep) {assert(!co_select._ntfSign.empty()); __selectStep=1;}\
	else if (1==__selectStep && co_select._ntfSign.empty()){__selectStep=0;co_select._selectId=-1;}}co_unlock_stop;}

//结束从多个channel/msg_buffer中以select方式读取数据(只能与co_begin_select_once配合)
#define co_end_select_once __selectCaseStep=0;__selectStep=1;}\
	co_end_switch; if (0==__selectStep) {assert(!co_select._ntfSign.empty());}}co_unlock_stop;}}

//结束select读取(只能在co_begin_select(__label__)/co_end_select中使用)
#define co_select_done(__label__) do{__selectCaseStep=3;__selectStep=1;__selectCaseDoSign=false;goto __select_ ## __label__;}while(0)

//锁定generator调度器
#define co_hold_work boost::asio::io_service::work __holdWork
#define co_hold_work_init __holdWork(co_strand->get_io_service())

struct co_context_base
{
#ifdef _MSC_VER
	virtual ~co_context_base(){}//FIXME VC下启用co_context_destroy时，不加这个会有编译错误
#endif
	void clear()
	{
		_sharedSign.reset();
		__coNext = 0;
		__readyQuit = false;
		__asyncSign = false;
#if (_DEBUG || DEBUG)
		__inside = false;
		__awaitSign = false;
		__sharedAwaitSign = false;
		__yieldSign = false;
#endif
	}
	shared_bool _sharedSign;
	int __coNext = 0;
	unsigned char __lockStop = 0;
	bool __readyQuit = false;
	bool __asyncSign = false;
#if (_DEBUG || DEBUG)
	bool __inside = false;
	bool __awaitSign = false;
	bool __sharedAwaitSign = false;
	bool __yieldSign = false;
#endif
};

class generator;
typedef std::shared_ptr<generator> generator_handle;

/*!
@brief stackless coroutine
*/
class generator : public ActorTimerFace_
{
	friend my_actor;
	FRIEND_SHARED_PTR(generator);
private:
	generator();
	~generator();
public:
	template <typename SharedStrand, typename Handler>
	static generator_handle create(SharedStrand&& strand, Handler&& handler)
	{
		mem_alloc_base* genObjRefCountAlloc_ = _genObjRefCountAlloc;
		generator_handle res(new(_genObjAlloc->allocate())generator(), [genObjRefCountAlloc_](generator* p)
		{
			p->~generator();
			genObjRefCountAlloc_->deallocate(p);
		});
		res->_weakThis = res;
		res->_strand = std::forward<SharedStrand>(strand);
		res->_handler = std::forward<Handler>(handler);
		res->_timer = res->_strand->actor_timer();
		return res;
	}

	template <typename SharedStrand, typename Handler, typename Notify>
	static generator_handle create(SharedStrand&& strand, Handler&& handler, Notify&& notify)
	{
		mem_alloc_base* genObjRefCountAlloc_ = _genObjRefCountAlloc;
		generator_handle res(new(_genObjAlloc->allocate())generator(), [genObjRefCountAlloc_](generator* p)
		{
			p->~generator();
			genObjRefCountAlloc_->deallocate(p);
		});
		res->_weakThis = res;
		res->_strand = std::forward<SharedStrand>(strand);
		res->_handler = std::forward<Handler>(handler);
		res->_notify = std::forward<Notify>(notify);
		res->_timer = res->_strand->actor_timer();
		return res;
	}

	void run();
	void stop();
	const shared_strand& gen_strand();
	generator_handle& shared_this();
	generator_handle& async_this();
	const shared_bool& shared_async_sign();
public:
	bool _next();
	void _lockThis();
	generator* _revert_this(generator_handle&);
	generator_handle _begin_fork(std::function<void()> notify = std::function<void()>());
	void _co_next();
	void _co_tick_next();
	void _co_async_next();
	void _co_shared_async_next(shared_bool& sign);

	template <typename Handler>
	bool _co_send(const shared_strand& strand, Handler&& handler)
	{
		if (gen_strand() != strand)
		{
			strand->post(std::bind([](generator_handle& gen, Handler& handler)
			{
				CHECK_EXCEPTION(handler);
				generator* const self = gen.get();
				self->gen_strand()->post(std::bind([](generator_handle& gen)
				{
					gen->_revert_this(gen)->_next();
				}, std::move(gen)));
			}, std::move(shared_this()), std::forward<Handler>(handler)));
			return true;
		}
		CHECK_EXCEPTION(handler);
		return false;
	}

	template <typename Handler>
	void _co_async_send(const shared_strand& strand, Handler&& handler)
	{
		strand->try_tick(std::bind([](generator_handle& gen, Handler& handler)
		{
			CHECK_EXCEPTION(handler);
			generator* const self = gen.get();
			self->gen_strand()->distribute(std::bind([](generator_handle& gen)
			{
				gen->_revert_this(gen)->_next();
			}, std::move(gen)));
		}, std::move(shared_this()), std::forward<Handler>(handler)));
	}

	co_context_base* _ctx;

	void _co_sleep(int ms);
private:
	void timeout_handler();
private:
	static void install();
	static void uninstall();
private:
	std::weak_ptr<generator> _weakThis;
	std::shared_ptr<generator> _sharedThis;
	std::function<void(generator&)> _handler;
	std::function<void()> _notify;
	shared_strand _strand;
	ActorTimer_* _timer;
	ActorTimer_::timer_handle _timerHandle;
	DEBUG_OPERATION(bool _isRun);
	NONE_COPY(generator);
	static mem_alloc_mt<generator>* _genObjAlloc;
	static mem_alloc_base* _genObjRefCountAlloc;
};

struct CoGo_
{
	CoGo_(shared_strand strand)
	:_strand(std::move(strand)) {}

	CoGo_(io_engine& ios)
	:_strand(boost_strand::create(ios)) {}

	template <typename Handler>
	generator_handle operator-(Handler&& handler)
	{
		generator_handle res = generator::create(std::move(_strand), std::forward<Handler>(handler));
		res->run();
		return res;
	}

	shared_strand _strand;
};

template <typename... _Types>
struct CoAsyncResult_
{
	CoAsyncResult_(generator_handle&& gen, _Types&... result)
	:_gen(std::move(gen)), _result(result...) {}

	template <typename... Args>
	void operator()(Args&&... args)
	{
		assert(_gen);
		_result = std::tuple<Args&&...>(std::forward<Args>(args)...);
		_gen->_revert_this(_gen)->_co_async_next();
	}

	generator_handle _gen;
	std::tuple<_Types&...> _result;
	void operator=(const CoAsyncResult_&) = delete;
	RVALUE_CONSTRUCT2(CoAsyncResult_, _gen, _result);
	LVALUE_CONSTRUCT2(CoAsyncResult_, _gen, _result);
};

template <typename... _Types>
struct CoAsyncSameResult_
{
	CoAsyncSameResult_(generator_handle&& gen, _Types&... result)
	:_gen(std::move(gen)), _result(result...) {}

	template <typename... Args>
	void operator()(Args&&... args)
	{
		assert(_gen);
		same_copy_to_tuple(_result, std::forward<Args>(args)...);
		_gen->_revert_this(_gen)->_co_async_next();
	}

	generator_handle _gen;
	std::tuple<_Types&...> _result;
	void operator=(const CoAsyncSameResult_&) = delete;
	RVALUE_CONSTRUCT2(CoAsyncSameResult_, _gen, _result);
	LVALUE_CONSTRUCT2(CoAsyncSameResult_, _gen, _result);
};

template <typename... _Types>
struct CoShardAsyncResult_
{
	CoShardAsyncResult_(generator_handle& gen, const shared_bool& sign, _Types&... result)
	:_gen(gen), _sign(sign), _result(result...) {}

	template <typename... Args>
	void operator()(Args&&... args)
	{
		_result = std::tuple<Args&&...>(std::forward<Args>(args)...);
		_gen->_co_shared_async_next(_sign);
	}

	generator_handle _gen;
	shared_bool _sign;
	std::tuple<_Types&...> _result;
	void operator=(const CoShardAsyncResult_&) = delete;
	RVALUE_CONSTRUCT3(CoShardAsyncResult_, _gen, _sign, _result);
	LVALUE_CONSTRUCT3(CoShardAsyncResult_, _gen, _sign, _result);
};

template <typename... _Types>
struct CoShardAsyncSameResult_
{
	CoShardAsyncSameResult_(generator_handle& gen, const shared_bool& sign, _Types&... result)
	:_gen(gen), _sign(sign), _result(result...) {}

	template <typename... Args>
	void operator()(Args&&... args)
	{
		same_copy_to_tuple(_result, std::forward<Args>(args)...);
		_gen->_co_shared_async_next(_sign);
	}

	generator_handle _gen;
	shared_bool _sign;
	std::tuple<_Types&...> _result;
	void operator=(const CoShardAsyncSameResult_&) = delete;
	RVALUE_CONSTRUCT3(CoShardAsyncSameResult_, _gen, _sign, _result);
	LVALUE_CONSTRUCT3(CoShardAsyncSameResult_, _gen, _sign, _result);
};

template <typename... _Types>
struct CoAnextResult_
{
	CoAnextResult_(generator_handle&& gen, _Types&... result)
	:_gen(std::move(gen)), _result(result...) {}

	template <typename... Args>
	void operator()(Args&&... args)
	{
		assert(_gen);
		_result = std::tuple<Args&&...>(std::forward<Args>(args)...);
		_gen->_revert_this(_gen)->_next();
	}

	generator_handle _gen;
	std::tuple<_Types&...> _result;
	void operator=(const CoAnextResult_&) = delete;
	RVALUE_CONSTRUCT2(CoAnextResult_, _gen, _result);
	LVALUE_CONSTRUCT2(CoAnextResult_, _gen, _result);
};

template <typename... _Types>
struct CoAnextSameResult_
{
	CoAnextSameResult_(generator_handle&& gen, _Types&... result)
	:_gen(std::move(gen)), _result(result...) {}

	template <typename... Args>
	void operator()(Args&&... args)
	{
		assert(_gen);
		same_copy_to_tuple(_result, std::forward<Args>(args)...);
		_gen->_revert_this(_gen)->_next();
	}

	generator_handle _gen;
	std::tuple<_Types&...> _result;
	void operator=(const CoAnextSameResult_&) = delete;
	RVALUE_CONSTRUCT2(CoAnextSameResult_, _gen, _result);
	LVALUE_CONSTRUCT2(CoAnextSameResult_, _gen, _result);
};

template <typename... Args>
CoAsyncResult_<Args...> _co_async_result(generator_handle&& gen, Args&... result)
{
	return CoAsyncResult_<Args...>(std::move(gen), result...);
}

template <typename... Args>
CoShardAsyncResult_<Args...> _co_shared_async_result(generator_handle& gen, const shared_bool& sign, Args&... result)
{
	return CoShardAsyncResult_<Args...>(gen, sign, result...);
}

template <typename... Args>
CoAnextResult_<Args...> _co_anext_result(generator_handle&& gen, Args&... result)
{
	return CoAnextResult_<Args...>(std::move(gen), result...);
}

template <typename... Args>
CoAsyncSameResult_<Args...> _co_async_same_result(generator_handle&& gen, Args&... result)
{
	return CoAsyncSameResult_<Args...>(std::move(gen), result...);
}

template <typename... Args>
CoShardAsyncSameResult_<Args...> _co_shared_async_same_result(generator_handle& gen, const shared_bool& sign, Args&... result)
{
	return CoShardAsyncSameResult_<Args...>(gen, sign, result...);
}

template <typename... Args>
CoAnextSameResult_<Args...> _co_anext_same_result(generator_handle&& gen, Args&... result)
{
	return CoAnextSameResult_<Args...>(std::move(gen), result...);
}
//////////////////////////////////////////////////////////////////////////

enum co_async_state
{
	co_async_undefined = -1,
	co_async_ok = 0,
	co_async_fail,
	co_async_cancel,
	co_async_closed,
	co_async_overtime
};

struct CoNotifyHandlerFace_
{
	virtual void notify(co_async_state) = 0;
	virtual void destroy() = 0;
	virtual size_t size() = 0;
	virtual CoNotifyHandlerFace_* move_out(void*) = 0;
	static void notify_state(reusable_mem& alloc, CoNotifyHandlerFace_* ntf, co_async_state state = co_async_ok);
};

template <typename Handler>
struct CoNotifyHandler_ : public CoNotifyHandlerFace_
{
	template <typename H>
	CoNotifyHandler_(H&& h)
		:_handler(std::forward<H>(h)) {}

	void notify(co_async_state state)
	{
		CHECK_EXCEPTION(_handler, state);
		destroy();
	}

	void destroy()
	{
		this->~CoNotifyHandler_();
	}

	size_t size()
	{
		return sizeof(CoNotifyHandler_);
	}

	CoNotifyHandlerFace_* move_out(void* space)
	{
		CoNotifyHandlerFace_* res = new(space)CoNotifyHandler_(std::move(*this));
		destroy();
		return res;
	}

	Handler _handler;
	NONE_COPY(CoNotifyHandler_);
	RVALUE_CONSTRUCT1(CoNotifyHandler_, _handler);
};

template <typename... Types>
struct CoChanMulti_ : public std::tuple<Types...>
{
	template <typename... Args>
	CoChanMulti_(Args&&... args)
		:std::tuple<Types...>(std::forward<Args>(args)...){}
};

template <typename... Type>
CoChanMulti_<Type&&...> _co_chan_multi(Type&&... args)
{
	return CoChanMulti_<Type&&...>(std::forward<Type>(args)...);
}

template <typename Chan>
struct CoChanIo_
{
	CoChanIo_(Chan& chan, co_async_state& state, generator& host)
	:_chan(chan), _state(state), _host(host) {}

	template <typename Arg>
	void operator<<(Arg&& arg)
	{
		push(this, std::forward<Arg>(arg));
	}

	template <typename Arg>
	void operator>>(Arg&& arg)
	{
		static_assert(!std::is_rvalue_reference<Arg&&>::value, "");
		pop(this, std::forward<Arg>(arg));
	}

	template <typename... Args>
	void operator<<(CoChanMulti_<Args...>&& args)
	{
		tuple_invoke(&CoChanIo_::push<Args...>, std::make_tuple(this), (std::tuple<Args...>&&)args);
	}

	template <typename... Args>
	void operator>>(CoChanMulti_<Args...>&& args)
	{
		tuple_invoke(&CoChanIo_::pop<Args...>, std::make_tuple(this), (std::tuple<Args...>&&)args);
	}

	void operator<<(void_type&&)
	{
		push(this);
	}

	void operator>>(void_type&&)
	{
		pop(this);
	}
private:
	template <typename... Args>
	static void push(CoChanIo_* this_, Args&&... args)
	{
		co_generator = this_->_host;
		this_->_chan.push(co_async_result(this_->_state), std::forward<Args>(args)...);
	}

	template <typename... Args>
	static void pop(CoChanIo_* this_, Args&&... args)
	{
		co_generator = this_->_host;
		this_->_chan.pop(co_async_result_(this_->_state, args...));
	}
private:
	Chan& _chan;
	co_async_state& _state;
	generator& _host;
};

template <typename Chan>
struct CoChanTryIo_
{
	CoChanTryIo_(Chan& chan, co_async_state& state, generator& host)
	:_chan(chan), _state(state), _host(host) {}

	template <typename Arg>
	void operator<<(Arg&& arg)
	{
		push(this, std::forward<Arg>(arg));
	}

	template <typename Arg>
	void operator>>(Arg&& arg)
	{
		static_assert(!std::is_rvalue_reference<Arg&&>::value, "");
		pop(this, std::forward<Arg>(arg));
	}

	template <typename... Args>
	void operator<<(CoChanMulti_<Args...>&& args)
	{
		tuple_invoke(&CoChanTryIo_::push<Args...>, std::make_tuple(this), (std::tuple<Args...>&&)args);
	}

	template <typename... Args>
	void operator>>(CoChanMulti_<Args...>&& args)
	{
		tuple_invoke(&CoChanTryIo_::pop<Args...>, std::make_tuple(this), (std::tuple<Args...>&&)args);
	}

	void operator<<(void_type&&)
	{
		push(this);
	}

	void operator>>(void_type&&)
	{
		pop(this);
	}
private:
	template <typename... Args>
	static void push(CoChanTryIo_* this_, Args&&... args)
	{
		co_generator = this_->_host;
		this_->_chan.try_push(co_async_result(this_->_state), std::forward<Args>(args)...);
	}

	template <typename... Args>
	static void pop(CoChanTryIo_* this_, Args&&... args)
	{
		co_generator = this_->_host;
		this_->_chan.try_pop(co_async_result_(this_->_state, args...));
	}
private:
	Chan& _chan;
	co_async_state& _state;
	generator& _host;
};

template <typename... Types>
class co_msg_buffer;
template <typename... Types>
class co_channel;
template <typename... Types>
class co_nil_channel;

typedef msg_list<CoNotifyHandlerFace_*>::iterator co_notify_node;

struct co_notify_sign
{
	co_notify_sign() :_ntfSign(true){}
	~co_notify_sign() { assert(_ntfSign); }
	co_notify_node _ntfNode;
	bool _ntfSign;
	NONE_COPY(co_notify_sign);
};

/*!
@brief 异步消息队列
*/
template <typename... Types>
class co_msg_buffer
{
	typedef std::tuple<TYPE_PIPE(Types)...> msg_type;
public:
	co_msg_buffer(const shared_strand& strand, size_t poolSize = sizeof(void*))
		:_closed(false), _strand(strand), _msgBuff(poolSize) {}

	~co_msg_buffer()
	{
		assert(_waitQueue.empty());
	}
public:
	template <typename Notify, typename... Args>
	void push(Notify&& ntf, Args&&... msg)
	{
		if (_strand->running_in_this_thread())
		{
			_push(msg_type(std::forward<Args>(msg)...), std::forward<Notify>(ntf));
		}
		else
		{
			_strand->post(std::bind([this](msg_type& msg, Notify& ntf)
			{
				_push(std::move(msg), std::forward<Notify>(ntf));
			}, msg_type(std::forward<Args>(msg)...), std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify>
	void pop(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_pop(std::forward<Notify>(ntf));
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_pop(std::forward<Notify>(ntf));
			}, std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify>
	void try_pop(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_try_pop(std::forward<Notify>(ntf));
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_try_pop(std::forward<Notify>(ntf));
			}, std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify>
	void timed_pop(async_timer& timer, int tm, Notify&& ntf)
	{
		assert(timer->self_strand() == _strand);
		if (_strand->running_in_this_thread())
		{
			_timed_pop(std::forward<Notify>(ntf), timer, tm * 1000);
		}
		else
		{
			_strand->post(std::bind([this, &timer, tm](Notify& ntf)
			{
				_timed_pop(std::forward<Notify>(ntf), timer, tm * 1000);
			}, std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify>
	void append_pop_notify(Notify&& ntf, co_notify_sign& ntfSign)
	{
		assert(ntfSign._ntfSign);
		ntfSign._ntfSign = false;
		if (_strand->running_in_this_thread())
		{
			_append_pop_notify(std::forward<Notify>(ntf), ntfSign);
		}
		else
		{
			_strand->post(std::bind([this, &ntfSign](Notify& ntf)
			{
				_append_pop_notify(std::forward<Notify>(ntf), ntfSign);
			}, std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify>
	void remove_pop_notify(Notify&& ntf, co_notify_sign& ntfSign)
	{
		if (_strand->running_in_this_thread())
		{
			_remove_pop_notify(std::forward<Notify>(ntf), ntfSign);
		}
		else
		{
			_strand->post(std::bind([this, &ntfSign](Notify& ntf)
			{
				_remove_pop_notify(std::forward<Notify>(ntf), ntfSign);
			}, std::forward<Notify>(ntf)));
		}
	}

	void close()
	{
		_strand->distribute([this]()
		{
			_close();
		});
	}

	template <typename Notify>
	void close(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_close();
			CHECK_EXCEPTION(ntf);
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_close();
				CHECK_EXCEPTION(ntf);
			}, std::forward<Notify>(ntf)));
		}
	}

	void cancel()
	{
		_strand->distribute([this]()
		{
			_cancel();
		});
	}

	template <typename Notify>
	void cancel(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_cancel();
			CHECK_EXCEPTION(ntf);
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_cancel();
				CHECK_EXCEPTION(ntf);
			}, std::forward<Notify>(ntf)));
		}
	}

	void reset()
	{
		assert(_waitQueue.empty());
		_closed = false;
	}

	const shared_strand& self_strand() const
	{
		return _strand;
	}
private:
	template <typename Handler>
	CoNotifyHandlerFace_* wrap_notify(Handler&& handler)
	{
		assert(_strand->running_in_this_thread());
		typedef CoNotifyHandler_<RM_CREF(Handler)> Handler_;
		return new(_alloc.allocate(sizeof(Handler_)))Handler_(std::forward<Handler>(handler));
	}

	template <typename Notify>
	void _push(msg_type&& msg, Notify&& ntf)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		_msgBuff.push_back(std::move(msg));
		if (!_waitQueue.empty())
		{
			assert(1 == _msgBuff.size());
			CoNotifyHandlerFace_* wtNtf = _waitQueue.front();
			_waitQueue.pop_front();
			CoNotifyHandlerFace_::notify_state(_alloc, wtNtf);
		}
		CHECK_EXCEPTION(ntf, co_async_state::co_async_ok);
	}

	template <typename Notify>
	void _pop(Notify&& ntf)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (!_msgBuff.empty())
		{
			msg_type msg(std::move(_msgBuff.front()));
			_msgBuff.pop_front();
			CHECK_EXCEPTION(tuple_invoke, ntf, std::tuple<co_async_state>(co_async_state::co_async_ok), std::move(msg));
		}
		else
		{
			_waitQueue.push_back(wrap_notify(std::bind([this](Notify& ntf, co_async_state state)
			{
				if (co_async_state::co_async_ok == state)
				{
					assert(!_msgBuff.empty());
					_pop(std::forward<Notify>(ntf));
				}
				else
				{
					CHECK_EXCEPTION(ntf, state);
				}
			}, std::forward<Notify>(ntf), __1)));
		}
	}

	template <typename Notify>
	void _try_pop(Notify&& ntf)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (!_msgBuff.empty())
		{
			msg_type msg(std::move(_msgBuff.front()));
			_msgBuff.pop_front();
			CHECK_EXCEPTION(tuple_invoke, ntf, std::tuple<co_async_state>(co_async_state::co_async_ok), std::move(msg));
		}
		else
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_fail);
		}
	}

	template <typename Notify>
	void _timed_pop(Notify&& ntf, async_timer& timer, long long tm)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (!_msgBuff.empty())
		{
			msg_type msg(std::move(_msgBuff.front()));
			_msgBuff.pop_front();
			CHECK_EXCEPTION(tuple_invoke, ntf, std::tuple<co_async_state>(co_async_state::co_async_ok), std::move(msg));
		}
		else if (tm > 0)
		{
			_waitQueue.push_back(wrap_notify(std::bind([this, &timer](Notify& ntf, co_async_state state)
			{
				timer->cancel();
				if (co_async_state::co_async_ok == state)
				{
					assert(!_msgBuff.empty());
					_pop(std::forward<Notify>(ntf));
				}
				else
				{
					CHECK_EXCEPTION(ntf, state);
				}
			}, std::forward<Notify>(ntf), __1)));
			timer->timeout((int)((tm + 999) / 1000), std::bind([this](const co_notify_node& it)
			{
				CoNotifyHandlerFace_* popNtf = *it;
				_waitQueue.erase(it);
				CoNotifyHandlerFace_::notify_state(_alloc, popNtf, co_async_state::co_async_overtime);
			}, --_waitQueue.end()));
		}
		else
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_overtime);
		}
	}

	template <typename Notify>
	void _append_pop_notify(Notify&& ntf, co_notify_sign& ntfSign)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (!_msgBuff.empty())
		{
			ntfSign._ntfSign = true;
			CHECK_EXCEPTION(ntf, co_async_state::co_async_ok);
		}
		else
		{
			_waitQueue.push_back(wrap_notify(std::bind([this, &ntfSign](Notify& ntf, co_async_state state)
			{
				assert(!ntfSign._ntfSign);
				ntfSign._ntfSign = true;
				CHECK_EXCEPTION(ntf, state);
			}, std::forward<Notify>(ntf), __1)));
			ntfSign._ntfNode = --_waitQueue.end();
		}
	}

	template <typename Notify>
	void _remove_pop_notify(Notify&& ntf, co_notify_sign& ntfSign)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (!ntfSign._ntfSign)
		{
			ntfSign._ntfSign = true;
			CoNotifyHandlerFace_* popNtf = *ntfSign._ntfNode;
			_waitQueue.erase(ntfSign._ntfNode);
			popNtf->destroy();
			_alloc.deallocate(popNtf);
			CHECK_EXCEPTION(ntf, co_async_state::co_async_ok);
		}
		else
		{
			if (!_msgBuff.empty() && !_waitQueue.empty())
			{
				CoNotifyHandlerFace_* wtNtf = _waitQueue.front();
				_waitQueue.pop_front();
				CoNotifyHandlerFace_::notify_state(_alloc, wtNtf);
			}
			CHECK_EXCEPTION(ntf, co_async_state::co_async_fail);
		}
	}

	void _close()
	{
		assert(_strand->running_in_this_thread());
		_closed = true;
		_msgBuff.clear();
		while (!_waitQueue.empty())
		{
			CoNotifyHandlerFace_* popNtf = _waitQueue.front();
			_waitQueue.pop_front();
			CoNotifyHandlerFace_::notify_state(_alloc, popNtf, co_async_state::co_async_closed);
		}
	}

	void _cancel()
	{
		assert(_strand->running_in_this_thread());
		while (!_waitQueue.empty())
		{
			CoNotifyHandlerFace_* popNtf = _waitQueue.front();
			_waitQueue.pop_front();
			CoNotifyHandlerFace_::notify_state(_alloc, popNtf, co_async_state::co_async_cancel);
		}
	}
private:
	shared_strand _strand;
	msg_queue<msg_type> _msgBuff;
	reusable_mem _alloc;
	msg_list<CoNotifyHandlerFace_*> _waitQueue;
	bool _closed;
	NONE_COPY(co_msg_buffer);
};

template <>
class co_msg_buffer<void> : public co_msg_buffer<void_type>
{
public:
	co_msg_buffer(const shared_strand& strand)
		:co_msg_buffer<void_type>(strand) {}
public:
	template <typename Notify> void push(Notify&& ntf){ co_msg_buffer<void_type>::push(std::forward<Notify>(ntf), void_type()); }
};

template <>
class co_msg_buffer<> : public co_msg_buffer<void>
{
public:
	co_msg_buffer(const shared_strand& strand)
		:co_msg_buffer<void>(strand) {}
};

/*!
@brief 异步channel通信
*/
template <typename... Types>
class co_channel
{
	typedef std::tuple<TYPE_PIPE(Types)...> msg_type;
public:
	co_channel(const shared_strand& strand, size_t buffLength = 1)
		:_closed(false), _strand(strand), _buffer(buffLength) {}

	~co_channel()
	{
		assert(_pushWait.empty());
		assert(_popWait.empty());
	}
public:
	template <typename Notify, typename... Args>
	void push(Notify&& ntf, Args&&... msg)
	{
		if (_strand->running_in_this_thread())
		{
			_push(msg_type(std::forward<Args>(msg)...), std::forward<Notify>(ntf));
		} 
		else
		{
			_strand->post(std::bind([this](msg_type& msg, Notify& ntf)
			{
				_push(std::move(msg), std::forward<Notify>(ntf));
			}, msg_type(std::forward<Args>(msg)...), std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify, typename... Args>
	void try_push(Notify&& ntf, Args&&... msg)
	{
		if (_strand->running_in_this_thread())
		{
			_try_push(msg_type(std::forward<Args>(msg)...), std::forward<Notify>(ntf));
		} 
		else
		{
			_strand->post(std::bind([this](msg_type& msg, Notify& ntf)
			{
				_try_push(std::move(msg), std::forward<Notify>(ntf));
			}, msg_type(std::forward<Args>(msg)...), std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify, typename... Args>
	void timed_push(async_timer& timer, int tm, Notify&& ntf, Args&&... msg)
	{
		assert(timer->self_strand() == _strand);
		if (_strand->running_in_this_thread())
		{
			_timed_push(msg_type(std::forward<Args>(msg)...), std::forward<Notify>(ntf), timer, tm * 1000);
		} 
		else
		{
			_strand->post(std::bind([this, &timer, tm](msg_type& msg, Notify& ntf)
			{
				_timed_push(std::move(msg), std::forward<Notify>(ntf), timer, tm * 1000);
			}, msg_type(std::forward<Args>(msg)...), std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify>
	void pop(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_pop(std::forward<Notify>(ntf));
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_pop(std::forward<Notify>(ntf));
			}, std::forward<Notify>(ntf)));
		}
	}
	
	template <typename Notify>
	void try_pop(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_try_pop(std::forward<Notify>(ntf));
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_try_pop(std::forward<Notify>(ntf));
			}, std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify>
	void timed_pop(async_timer& timer, int tm, Notify&& ntf)
	{
		assert(timer->self_strand() == _strand);
		if (_strand->running_in_this_thread())
		{
			_timed_pop(std::forward<Notify>(ntf), timer, tm * 1000);
		}
		else
		{
			_strand->post(std::bind([this, &timer, tm](Notify& ntf)
			{
				_timed_pop(std::forward<Notify>(ntf), timer, tm * 1000);
			}, std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify>
	void append_pop_notify(Notify&& ntf, co_notify_sign& ntfSign)
	{
		assert(ntfSign._ntfSign);
		ntfSign._ntfSign = false;
		if (_strand->running_in_this_thread())
		{
			_append_pop_notify(std::forward<Notify>(ntf), ntfSign);
		}
		else
		{
			_strand->post(std::bind([this, &ntfSign](Notify& ntf)
			{
				_append_pop_notify(std::forward<Notify>(ntf), ntfSign);
			}, std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify>
	void remove_pop_notify(Notify&& ntf, co_notify_sign& ntfSign)
	{
		if (_strand->running_in_this_thread())
		{
			_remove_pop_notify(std::forward<Notify>(ntf), ntfSign);
		}
		else
		{
			_strand->post(std::bind([this, &ntfSign](Notify& ntf)
			{
				_remove_pop_notify(std::forward<Notify>(ntf), ntfSign);
			}, std::forward<Notify>(ntf)));
		}
	}

	void close()
	{
		_strand->distribute([this]()
		{
			_close();
		});
	}

	template <typename Notify>
	void close(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_close();
			CHECK_EXCEPTION(ntf);
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_close();
				CHECK_EXCEPTION(ntf);
			}, std::forward<Notify>(ntf)));
		}
	}

	void cancel()
	{
		_strand->distribute([this]()
		{
			_cancel();
		});
	}

	template <typename Notify>
	void cancel(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_cancel();
			CHECK_EXCEPTION(ntf);
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_cancel();
				CHECK_EXCEPTION(ntf);
			}, std::forward<Notify>(ntf)));
		}
	}

	void cancel_push()
	{
		_strand->distribute([this]()
		{
			_cancel_push();
		});
	}

	template <typename Notify>
	void cancel_push(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_cancel_push();
			CHECK_EXCEPTION(ntf);
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_cancel_push();
				CHECK_EXCEPTION(ntf);
			}, std::forward<Notify>(ntf)));
		}
	}

	void cancel_pop()
	{
		_strand->distribute([this]()
		{
			_cancel_pop();
		});
	}

	template <typename Notify>
	void cancel_pop(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_cancel_pop();
			CHECK_EXCEPTION(ntf);
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_cancel_pop();
				CHECK_EXCEPTION(ntf);
			}, std::forward<Notify>(ntf)));
		}
	}

	void reset()
	{
		assert(_pushWait.empty());
		assert(_popWait.empty());
		_closed = false;
	}

	const shared_strand& self_strand() const
	{
		return _strand;
	}
private:
	template <typename Handler>
	CoNotifyHandlerFace_* wrap_notify(Handler&& handler)
	{
		assert(_strand->running_in_this_thread());
		typedef CoNotifyHandler_<RM_CREF(Handler)> Handler_;
		return new(_alloc.allocate(sizeof(Handler_)))Handler_(std::forward<Handler>(handler));
	}

	template <typename Notify>
	void _push(msg_type&& msg, Notify&& ntf)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (_buffer.full())
		{
			_pushWait.push_back(wrap_notify(std::bind([this](msg_type& msg, Notify& ntf, co_async_state state)
			{
				if (co_async_state::co_async_ok == state)
				{
					assert(!_buffer.full());
					_push(std::move(msg), std::forward<Notify>(ntf));
				}
				else
				{
					CHECK_EXCEPTION(ntf, state);
				}
			}, std::move(msg), std::forward<Notify>(ntf), __1)));
		}
		else
		{
			assert(_pushWait.empty());
			_buffer.push_back(std::move(msg));
			if (!_popWait.empty())
			{
				assert(1 == _buffer.size());
				CoNotifyHandlerFace_* popNtf = _popWait.front();
				_popWait.pop_front();
				CoNotifyHandlerFace_::notify_state(_alloc, popNtf);
			}
			CHECK_EXCEPTION(ntf, co_async_state::co_async_ok);
		}
	}

	template <typename Notify>
	void _try_push(msg_type&& msg, Notify&& ntf)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (_buffer.full())
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_fail);
		}
		else
		{
			assert(_pushWait.empty());
			_buffer.push_back(std::move(msg));
			if (!_popWait.empty())
			{
				assert(1 == _buffer.size());
				CoNotifyHandlerFace_* popNtf = _popWait.front();
				_popWait.pop_front();
				CoNotifyHandlerFace_::notify_state(_alloc, popNtf);
			}
			CHECK_EXCEPTION(ntf, co_async_state::co_async_ok);
		}
	}

	template <typename Notify>
	void _timed_push(msg_type&& msg, Notify&& ntf, async_timer& timer, long long tm)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (_buffer.full())
		{
			if (tm > 0)
			{
				_pushWait.push_back(wrap_notify(std::bind([this, &timer](msg_type& msg, Notify& ntf, co_async_state state)
				{
					timer->cancel();
					if (co_async_state::co_async_ok == state)
					{
						assert(!_buffer.full());
						_push(std::move(msg), std::forward<Notify>(ntf));
					}
					else
					{
						CHECK_EXCEPTION(ntf, state);
					}
				}, std::move(msg), std::forward<Notify>(ntf), __1)));
				timer->timeout((int)((tm + 999) / 1000), std::bind([this](const co_notify_node& it)
				{
					CoNotifyHandlerFace_* pushWait = *it;
					_pushWait.erase(it);
					CoNotifyHandlerFace_::notify_state(_alloc, pushWait, co_async_state::co_async_overtime);
				}, --_pushWait.end()));
			}
			else
			{
				CHECK_EXCEPTION(ntf, co_async_state::co_async_overtime);
			}
		}
		else
		{
			assert(_pushWait.empty());
			_buffer.push_back(std::move(msg));
			if (!_popWait.empty())
			{
				assert(1 == _buffer.size());
				CoNotifyHandlerFace_* popNtf = _popWait.front();
				_popWait.pop_front();
				CoNotifyHandlerFace_::notify_state(_alloc, popNtf);
			}
			CHECK_EXCEPTION(ntf, co_async_state::co_async_ok);
		}
	}

	template <typename Notify>
	void _pop(Notify&& ntf)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (!_buffer.empty())
		{
			msg_type msg(std::move(_buffer.front()));
			_buffer.pop_front();
			if (!_pushWait.empty())
			{
				CoNotifyHandlerFace_* pushNtf = _pushWait.front();
				_pushWait.pop_front();
				CoNotifyHandlerFace_::notify_state(_alloc, pushNtf);
			}
			CHECK_EXCEPTION(tuple_invoke, ntf, std::tuple<co_async_state>(co_async_state::co_async_ok), std::move(msg));
		}
		else
		{
			assert(_pushWait.empty());
			_popWait.push_back(wrap_notify(std::bind([this](Notify& ntf, co_async_state state)
			{
				if (co_async_state::co_async_ok == state)
				{
					assert(!_buffer.empty());
					_pop(std::forward<Notify>(ntf));
				}
				else
				{
					CHECK_EXCEPTION(ntf, state);
				}
			}, std::forward<Notify>(ntf), __1)));
		}
	}

	template <typename Notify>
	void _try_pop(Notify&& ntf)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (!_buffer.empty())
		{
			msg_type msg(std::move(_buffer.front()));
			_buffer.pop_front();
			if (!_pushWait.empty())
			{
				CoNotifyHandlerFace_* pushNtf = _pushWait.front();
				_pushWait.pop_front();
				CoNotifyHandlerFace_::notify_state(_alloc, pushNtf);
			}
			CHECK_EXCEPTION(tuple_invoke, ntf, std::tuple<co_async_state>(co_async_state::co_async_ok), std::move(msg));
		}
		else
		{
			assert(_pushWait.empty());
			CHECK_EXCEPTION(ntf, co_async_state::co_async_fail);
		}
	}

	template <typename Notify>
	void _timed_pop(Notify&& ntf, async_timer& timer, long long tm)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (!_buffer.empty())
		{
			msg_type msg(std::move(_buffer.front()));
			_buffer.pop_front();
			if (!_pushWait.empty())
			{
				CoNotifyHandlerFace_* pushNtf = _pushWait.front();
				_pushWait.pop_front();
				CoNotifyHandlerFace_::notify_state(_alloc, pushNtf);
			}
			CHECK_EXCEPTION(tuple_invoke, ntf, std::tuple<co_async_state>(co_async_state::co_async_ok), std::move(msg));
		}
		else if (tm > 0)
		{
			assert(_pushWait.empty());
			_popWait.push_back(wrap_notify(std::bind([this, &timer](Notify& ntf, co_async_state state)
			{
				timer->cancel();
				if (co_async_state::co_async_ok == state)
				{
					assert(!_buffer.empty());
					_pop(std::forward<Notify>(ntf));
				}
				else
				{
					CHECK_EXCEPTION(ntf, state);
				}
			}, std::forward<Notify>(ntf), __1)));
			timer->timeout((int)((tm + 999) / 1000), std::bind([this](const co_notify_node& it)
			{
				CoNotifyHandlerFace_* popWait = *it;
				_popWait.erase(it);
				CoNotifyHandlerFace_::notify_state(_alloc, popWait, co_async_state::co_async_overtime);
			}, --_popWait.end()));
		}
		else
		{
			assert(_pushWait.empty());
			CHECK_EXCEPTION(ntf, co_async_state::co_async_overtime);
		}
	}

	template <typename Notify>
	void _append_pop_notify(Notify&& ntf, co_notify_sign& ntfSign)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (!_buffer.empty())
		{
			ntfSign._ntfSign = true;
			CHECK_EXCEPTION(ntf, co_async_state::co_async_ok);
		}
		else
		{
			assert(_pushWait.empty());
			_popWait.push_back(wrap_notify(std::bind([&ntfSign](Notify& ntf, co_async_state state)
			{
				assert(!ntfSign._ntfSign);
				ntfSign._ntfSign = true;
				CHECK_EXCEPTION(ntf, state);
			}, std::forward<Notify>(ntf), __1)));
			ntfSign._ntfNode = --_popWait.end();
		}
	}

	template <typename Notify>
	void _remove_pop_notify(Notify&& ntf, co_notify_sign& ntfSign)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (!ntfSign._ntfSign)
		{
			ntfSign._ntfSign = true;
			CoNotifyHandlerFace_* popNtf = *ntfSign._ntfNode;
			_popWait.erase(ntfSign._ntfNode);
			popNtf->destroy();
			_alloc.deallocate(popNtf);
			CHECK_EXCEPTION(ntf, co_async_state::co_async_ok);
		}
		else
		{
			if (!_buffer.empty() && !_popWait.empty())
			{
				CoNotifyHandlerFace_* popNtf = _popWait.front();
				_popWait.pop_front();
				CoNotifyHandlerFace_::notify_state(_alloc, popNtf);
			}
			CHECK_EXCEPTION(ntf, co_async_state::co_async_fail);
		}
	}

	void _close()
	{
		assert(_strand->running_in_this_thread());
		_closed = true;
		_buffer.clear();
		while (!_pushWait.empty())
		{
			CoNotifyHandlerFace_* pushNtf = _pushWait.front();
			_pushWait.pop_front();
			CoNotifyHandlerFace_::notify_state(_alloc, pushNtf, co_async_state::co_async_closed);
		}
		while (!_popWait.empty())
		{
			CoNotifyHandlerFace_* popNtf = _popWait.front();
			_popWait.pop_front();
			CoNotifyHandlerFace_::notify_state(_alloc, popNtf, co_async_state::co_async_closed);
		}
	}

	void _cancel()
	{
		_cancel_push();
		_cancel_pop();
	}

	void _cancel_push()
	{
		assert(_strand->running_in_this_thread());
		while (!_pushWait.empty())
		{
			CoNotifyHandlerFace_* pushNtf = _pushWait.front();
			_pushWait.pop_front();
			CoNotifyHandlerFace_::notify_state(_alloc, pushNtf, co_async_state::co_async_cancel);
		}
	}

	void _cancel_pop()
	{
		assert(_strand->running_in_this_thread());
		while (!_popWait.empty())
		{
			CoNotifyHandlerFace_* popNtf = _popWait.front();
			_popWait.pop_front();
			CoNotifyHandlerFace_::notify_state(_alloc, popNtf, co_async_state::co_async_cancel);
		}
	}
private:
	shared_strand _strand;
	fixed_buffer<msg_type> _buffer;
	reusable_mem _alloc;
	msg_list<CoNotifyHandlerFace_*> _pushWait;
	msg_list<CoNotifyHandlerFace_*> _popWait;
	bool _closed;
	NONE_COPY(co_channel);
};

template <>
class co_channel<void> : public co_channel<void_type>
{
public:
	co_channel(const shared_strand& strand, size_t buffLength = 1)
		:co_channel<void_type>(strand, buffLength) {}
public:
	template <typename Notify> void push(Notify&& ntf){ co_channel<void_type>::push(std::forward<Notify>(ntf), void_type()); }
	template <typename Notify> void try_push(Notify&& ntf){ co_channel<void_type>::try_push(std::forward<Notify>(ntf), void_type()); }
	template <typename Notify> void timed_push(async_timer& timer, int tm, Notify&& ntf){ co_channel<void_type>::timed_push(timer, tm, std::forward<Notify>(ntf), void_type()); }
};

template <>
class co_channel<> : public co_channel<void>
{
public:
	co_channel(const shared_strand& strand, size_t buffLength = 1)
		:co_channel<void>(strand, buffLength) {}
};

/*!
@brief 异步无缓冲channel通信
*/
template <typename... Types>
class co_nil_channel
{
	typedef std::tuple<TYPE_PIPE(Types)...> msg_type;
public:
	co_nil_channel(const shared_strand& strand)
		:_closed(false), _strand(strand) {}

	~co_nil_channel()
	{
		assert(_pushWait.empty());
		assert(_popWait.empty());
	}
public:
	template <typename Notify, typename... Args>
	void push(Notify&& ntf, Args&&... msg)
	{
		if (_strand->running_in_this_thread())
		{
			_push(msg_type(std::forward<Args>(msg)...), std::forward<Notify>(ntf));
		}
		else
		{
			_strand->post(std::bind([this](msg_type& msg, Notify& ntf)
			{
				_push(std::move(msg), std::forward<Notify>(ntf));
			}, msg_type(std::forward<Args>(msg)...), std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify, typename... Args>
	void try_push(Notify&& ntf, Args&&... msg)
	{
		if (_strand->running_in_this_thread())
		{
			_try_push(msg_type(std::forward<Args>(msg)...), std::forward<Notify>(ntf));
		}
		else
		{
			_strand->post(std::bind([this](msg_type& msg, Notify& ntf)
			{
				_try_push(std::move(msg), std::forward<Notify>(ntf));
			}, msg_type(std::forward<Args>(msg)...), std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify, typename... Args>
	void timed_push(async_timer& timer, int tm, Notify&& ntf, Args&&... msg)
	{
		assert(timer->self_strand() == _strand);
		if (_strand->running_in_this_thread())
		{
			_timed_push(msg_type(std::forward<Args>(msg)...), std::forward<Notify>(ntf), timer, tm * 1000);
		}
		else
		{
			_strand->post(std::bind([this, &timer, tm](msg_type& msg, Notify& ntf)
			{
				_timed_push(std::move(msg), std::forward<Notify>(ntf), timer, tm * 1000);
			}, msg_type(std::forward<Args>(msg)...), std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify>
	void pop(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_pop(std::forward<Notify>(ntf));
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_pop(std::forward<Notify>(ntf));
			}, std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify>
	void try_pop(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_try_pop(std::forward<Notify>(ntf));
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_try_pop(std::forward<Notify>(ntf));
			}, std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify>
	void timed_pop(async_timer& timer, int tm, Notify&& ntf)
	{
		assert(timer->self_strand() == _strand);
		if (_strand->running_in_this_thread())
		{
			_timed_pop(std::forward<Notify>(ntf), timer, tm * 1000);
		}
		else
		{
			_strand->post(std::bind([this, &timer, tm](Notify& ntf)
			{
				_timed_pop(std::forward<Notify>(ntf), timer, tm * 1000);
			}, std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify>
	void append_pop_notify(Notify&& ntf, co_notify_sign& ntfSign)
	{
		assert(ntfSign._ntfSign);
		ntfSign._ntfSign = false;
		if (_strand->running_in_this_thread())
		{
			_append_pop_notify(std::forward<Notify>(ntf), ntfSign);
		}
		else
		{
			_strand->post(std::bind([this, &ntfSign](Notify& ntf)
			{
				_append_pop_notify(std::forward<Notify>(ntf), ntfSign);
			}, std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify>
	void remove_pop_notify(Notify&& ntf, co_notify_sign& ntfSign)
	{
		if (_strand->running_in_this_thread())
		{
			_remove_pop_notify(std::forward<Notify>(ntf), ntfSign);
		}
		else
		{
			_strand->post(std::bind([this, &ntfSign](Notify& ntf)
			{
				_remove_pop_notify(std::forward<Notify>(ntf), ntfSign);
			}, std::forward<Notify>(ntf)));
		}
	}

	void close()
	{
		_strand->distribute([this]()
		{
			_close();
		});
	}

	template <typename Notify>
	void close(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_close();
			CHECK_EXCEPTION(ntf);
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_close();
				CHECK_EXCEPTION(ntf);
			}, std::forward<Notify>(ntf)));
		}
	}

	void cancel()
	{
		_strand->distribute([this]()
		{
			_cancel();
		});
	}

	template <typename Notify>
	void cancel(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_cancel();
			CHECK_EXCEPTION(ntf);
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_cancel();
				CHECK_EXCEPTION(ntf);
			}, std::forward<Notify>(ntf)));
		}
	}

	void cancel_push()
	{
		_strand->distribute([this]()
		{
			_cancel_push();
		});
	}

	template <typename Notify>
	void cancel_push(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_cancel_push();
			CHECK_EXCEPTION(ntf);
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_cancel_push();
				CHECK_EXCEPTION(ntf);
			}, std::forward<Notify>(ntf)));
		}
	}

	void cancel_pop()
	{
		_strand->distribute([this]()
		{
			_cancel_pop();
		});
	}

	template <typename Notify>
	void cancel_pop(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_cancel_pop();
			CHECK_EXCEPTION(ntf);
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_cancel_pop();
				CHECK_EXCEPTION(ntf);
			}, std::forward<Notify>(ntf)));
		}
	}

	void reset()
	{
		assert(_pushWait.empty());
		assert(_popWait.empty());
		_closed = false;
	}

	const shared_strand& self_strand() const
	{
		return _strand;
	}
private:
	template <typename Handler>
	CoNotifyHandlerFace_* wrap_notify(Handler&& handler)
	{
		assert(_strand->running_in_this_thread());
		typedef CoNotifyHandler_<RM_CREF(Handler)> Handler_;
		return new(_alloc.allocate(sizeof(Handler_)))Handler_(std::forward<Handler>(handler));
	}
	
	template <typename Notify>
	void _push(msg_type&& msg, Notify&& ntf)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (_tempBuffer.has())
		{
			_pushWait.push_back(wrap_notify(std::bind([this](msg_type& msg, Notify& ntf, co_async_state state)
			{
				if (co_async_state::co_async_ok == state)
				{
					assert(!_tempBuffer.has());
					_push(std::move(msg), std::forward<Notify>(ntf));
				}
				else
				{
					CHECK_EXCEPTION(ntf, state);
				}
			}, std::move(msg), std::forward<Notify>(ntf), __1)));
		}
		else
		{
			assert(_pushWait.empty());
			_tempBuffer.create(std::move(msg));
			_pushWait.push_back(wrap_notify(std::bind([this](Notify& ntf, co_async_state state)
			{
				if (co_async_state::co_async_ok == state && !_pushWait.empty())
				{
					CoNotifyHandlerFace_* pushNtf = _pushWait.front();
					_pushWait.pop_front();
					CoNotifyHandlerFace_::notify_state(_alloc, pushNtf);
				}
				CHECK_EXCEPTION(ntf, state);
			}, std::forward<Notify>(ntf), __1)));
			if (!_popWait.empty())
			{
				CoNotifyHandlerFace_* popNtf = _popWait.front();
				_popWait.pop_front();
				CoNotifyHandlerFace_::notify_state(_alloc, popNtf);
			}
		}
	}
	
	template <typename Notify>
	void _try_push(msg_type&& msg, Notify&& ntf)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (_tempBuffer.has())
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_fail);
		}
		else
		{
			assert(_pushWait.empty());
			_tempBuffer.create(std::move(msg));
			_pushWait.push_back(wrap_notify(std::bind([this](Notify& ntf, co_async_state state)
			{
				if (co_async_state::co_async_ok == state && !_pushWait.empty())
				{
					CoNotifyHandlerFace_* pushNtf = _pushWait.front();
					_pushWait.pop_front();
					CoNotifyHandlerFace_::notify_state(_alloc, pushNtf);
				}
				CHECK_EXCEPTION(ntf, state);
			}, std::forward<Notify>(ntf), __1)));
			if (!_popWait.empty())
			{
				CoNotifyHandlerFace_* popNtf = _popWait.front();
				_popWait.pop_front();
				CoNotifyHandlerFace_::notify_state(_alloc, popNtf);
			}
		}
	}

	template <typename Notify>
	void _timed_push(msg_type&& msg, Notify&& ntf, async_timer& timer, long long tm)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (_tempBuffer.has())
		{
			if (tm > 0)
			{
				_pushWait.push_back(wrap_notify(std::bind([this, &timer](msg_type& msg, Notify& ntf, co_async_state state)
				{
					timer->cancel();
					if (co_async_state::co_async_ok == state)
					{
						assert(!_tempBuffer.has());
						_push(std::move(msg), std::forward<Notify>(ntf));
					}
					else
					{
						CHECK_EXCEPTION(ntf, state);
					}
				}, std::move(msg), std::forward<Notify>(ntf), __1)));
				timer->timeout((int)((tm + 999) / 1000), std::bind([this](const co_notify_node& it)
				{
					CoNotifyHandlerFace_* pushWait = *it;
					_pushWait.erase(it);
					CoNotifyHandlerFace_::notify_state(_alloc, pushWait, co_async_state::co_async_overtime);
				}, --_pushWait.end()));
			}
			else
			{
				CHECK_EXCEPTION(ntf, co_async_state::co_async_overtime);
			}
		}
		else
		{
			assert(_pushWait.empty());
			_tempBuffer.create(std::move(msg));
			_pushWait.push_back(wrap_notify(std::bind([this](Notify& ntf, co_async_state state)
			{
				if (co_async_state::co_async_ok == state && !_pushWait.empty())
				{
					CoNotifyHandlerFace_* pushNtf = _pushWait.front();
					_pushWait.pop_front();
					CoNotifyHandlerFace_::notify_state(_alloc, pushNtf);
				}
				CHECK_EXCEPTION(ntf, state);
			}, std::forward<Notify>(ntf), __1)));
			if (!_popWait.empty())
			{
				CoNotifyHandlerFace_* popNtf = _popWait.front();
				_popWait.pop_front();
				CoNotifyHandlerFace_::notify_state(_alloc, popNtf);
			}
		}
	}

	template <typename Notify>
	void _pop(Notify&& ntf)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (_tempBuffer.has())
		{
			assert(!_pushWait.empty());
			msg_type msg(std::move(_tempBuffer.get()));
			_tempBuffer.destroy();
			CoNotifyHandlerFace_* pushNtf = _pushWait.front();
			_pushWait.pop_front();
			CoNotifyHandlerFace_::notify_state(_alloc, pushNtf);
			CHECK_EXCEPTION(tuple_invoke, ntf, std::tuple<co_async_state>(co_async_state::co_async_ok), std::move(msg));
		}
		else
		{
			assert(_pushWait.empty());
			_popWait.push_back(wrap_notify(std::bind([this](Notify& ntf, co_async_state state)
			{
				if (co_async_state::co_async_ok == state)
				{
					_pop(std::forward<Notify>(ntf));
				}
				else
				{
					CHECK_EXCEPTION(ntf, state);
				}
			}, std::forward<Notify>(ntf), __1)));
		}
	}
	
	template <typename Notify>
	void _try_pop(Notify&& ntf)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (_tempBuffer.has())
		{
			assert(!_pushWait.empty());
			msg_type msg(std::move(_tempBuffer.get()));
			_tempBuffer.destroy();
			CoNotifyHandlerFace_* pushNtf = _pushWait.front();
			_pushWait.pop_front();
			CoNotifyHandlerFace_::notify_state(_alloc, pushNtf);
			CHECK_EXCEPTION(tuple_invoke, ntf, std::tuple<co_async_state>(co_async_state::co_async_ok), std::move(msg));
		}
		else
		{
			assert(_pushWait.empty());
			CHECK_EXCEPTION(ntf, co_async_state::co_async_fail);
		}
	}

	template <typename Notify>
	void _timed_pop(Notify&& ntf, async_timer& timer, long long tm)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (_tempBuffer.has())
		{
			assert(!_pushWait.empty());
			msg_type msg(std::move(_tempBuffer.get()));
			_tempBuffer.destroy();
			CoNotifyHandlerFace_* pushNtf = _pushWait.front();
			_pushWait.pop_front();
			CoNotifyHandlerFace_::notify_state(_alloc, pushNtf);
			CHECK_EXCEPTION(tuple_invoke, ntf, std::tuple<co_async_state>(co_async_state::co_async_ok), std::move(msg));
		}
		else if (tm > 0)
		{
			assert(_pushWait.empty());
			_popWait.push_back(wrap_notify(std::bind([this, &timer](Notify& ntf, co_async_state state)
			{
				timer->cancel();
				if (co_async_state::co_async_ok == state)
				{
					assert(_tempBuffer.has());
					_pop(std::forward<Notify>(ntf));
				}
				else
				{
					CHECK_EXCEPTION(ntf, state);
				}
			}, std::forward<Notify>(ntf), __1)));
			timer->timeout((int)((tm + 999) / 1000), std::bind([this](const co_notify_node& it)
			{
				CoNotifyHandlerFace_* popWait = *it;
				_popWait.erase(it);
				CoNotifyHandlerFace_::notify_state(_alloc, popWait, co_async_state::co_async_overtime);
			}, --_popWait.end()));
		}
		else
		{
			assert(_pushWait.empty());
			CHECK_EXCEPTION(ntf, co_async_state::co_async_overtime);
		}
	}

	template <typename Notify>
	void _append_pop_notify(Notify&& ntf, co_notify_sign& ntfSign)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (_tempBuffer.has())
		{
			ntfSign._ntfSign = true;
			CHECK_EXCEPTION(ntf, co_async_state::co_async_ok);
		}
		else
		{
			assert(_pushWait.empty());
			_popWait.push_back(wrap_notify(std::bind([&ntfSign](Notify& ntf, co_async_state state)
			{
				assert(!ntfSign._ntfSign);
				ntfSign._ntfSign = true;
				CHECK_EXCEPTION(ntf, state);
			}, std::forward<Notify>(ntf), __1)));
			ntfSign._ntfNode = --_popWait.end();
		}
	}

	template <typename Notify>
	void _remove_pop_notify(Notify&& ntf, co_notify_sign& ntfSign)
	{
		assert(_strand->running_in_this_thread());
		if (_closed)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_closed);
			return;
		}
		if (!ntfSign._ntfSign)
		{
			ntfSign._ntfSign = true;
			CoNotifyHandlerFace_* popNtf = *ntfSign._ntfNode;
			_popWait.erase(ntfSign._ntfNode);
			popNtf->destroy();
			_alloc.deallocate(popNtf);
			CHECK_EXCEPTION(ntf, co_async_state::co_async_ok);
		}
		else
		{
			if (_tempBuffer.has() && !_popWait.empty())
			{
				CoNotifyHandlerFace_* popNtf = _popWait.front();
				_popWait.pop_front();
				CoNotifyHandlerFace_::notify_state(_alloc, popNtf);
			}
			CHECK_EXCEPTION(ntf, co_async_state::co_async_fail);
		}
	}

	void _close()
	{
		assert(_strand->running_in_this_thread());
		_closed = true;
		_tempBuffer.destroy();
		while (!_pushWait.empty())
		{
			CoNotifyHandlerFace_* pushNtf = _pushWait.front();
			_pushWait.pop_front();
			CoNotifyHandlerFace_::notify_state(_alloc, pushNtf, co_async_state::co_async_closed);
		}
		while (!_popWait.empty())
		{
			CoNotifyHandlerFace_* popNtf = _popWait.front();
			_popWait.pop_front();
			CoNotifyHandlerFace_::notify_state(_alloc, popNtf, co_async_state::co_async_closed);
		}
	}

	void _cancel()
	{
		_cancel_push();
		_cancel_pop();
	}

	void _cancel_push()
	{
		assert(_strand->running_in_this_thread());
		while (!_pushWait.empty())
		{
			CoNotifyHandlerFace_* pushNtf = _pushWait.front();
			_pushWait.pop_front();
			CoNotifyHandlerFace_::notify_state(_alloc, pushNtf, co_async_state::co_async_cancel);
		}
	}

	void _cancel_pop()
	{
		assert(_strand->running_in_this_thread());
		while (!_popWait.empty())
		{
			CoNotifyHandlerFace_* popNtf = _popWait.front();
			_popWait.pop_front();
			CoNotifyHandlerFace_::notify_state(_alloc, popNtf, co_async_state::co_async_cancel);
		}
	}
private:
	shared_strand _strand;
	stack_obj<msg_type> _tempBuffer;
	reusable_mem _alloc;
	msg_list<CoNotifyHandlerFace_*> _pushWait;
	msg_list<CoNotifyHandlerFace_*> _popWait;
	bool _closed;
	NONE_COPY(co_nil_channel);
};

template <>
class co_nil_channel<void> : public co_nil_channel<void_type>
{
public:
	co_nil_channel(const shared_strand& strand)
		:co_nil_channel<void_type>(strand) {}
public:
	template <typename Notify> void push(Notify&& ntf){ co_nil_channel<void_type>::push(std::forward<Notify>(ntf), void_type()); }
	template <typename Notify> void try_push(Notify&& ntf){ co_nil_channel<void_type>::try_push(std::forward<Notify>(ntf), void_type()); }
	template <typename Notify> void timed_push(async_timer& timer, int tm, Notify&& ntf){ co_nil_channel<void_type>::timed_push(timer, tm, std::forward<Notify>(ntf), void_type()); }
};

template <>
class co_nil_channel<> : public co_nil_channel<void>
{
public:
	co_nil_channel(const shared_strand& strand)
		:co_nil_channel<void>(strand) {}
};

struct CoSelectSign_
{
	CoSelectSign_(co_generator)
	:_ntfPump(co_strand), _ntfSign(16), _ntfState(co_async_state::co_async_undefined), _selectId(-1) {}
	CoSelectSign_(const CoSelectSign_& s)
		:_ntfPump(s._ntfPump.self_strand()), _ntfSign(16), _ntfState(s._ntfState), _selectId(s._selectId) {}

	co_msg_buffer<int> _ntfPump;
	msg_map<size_t, co_notify_sign> _ntfSign;
	co_async_state _ntfState;
	int _selectId;
};

/*!
@brief generator中逻辑mutex同步
*/
class co_mutex
{
public:
	co_mutex(const shared_strand& strand)
		:_locked(false), _strand(strand){}

	~co_mutex()
	{
	}
public:
	template <typename Notify>
	void lock(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_lock(std::forward<Notify>(ntf));
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_lock(std::forward<Notify>(ntf));
			}, std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify>
	void try_lock(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_try_lock(std::forward<Notify>(ntf));
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_try_lock(std::forward<Notify>(ntf));
			}, std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify>
	void timed_lock(async_timer& timer, int tm, Notify&& ntf)
	{
		assert(timer->self_strand() == _strand);
		if (_strand->running_in_this_thread())
		{
			_timed_lock(timer, tm, std::forward<Notify>(ntf));
		}
		else
		{
			_strand->post(std::bind([this, &timer, tm](Notify& ntf)
			{
				_timed_lock(timer, tm, std::forward<Notify>(ntf));
			}, std::forward<Notify>(ntf)));
		}
	}

	template <typename Notify>
	void unlock(Notify&& ntf)
	{
		if (_strand->running_in_this_thread())
		{
			_unlock(std::forward<Notify>(ntf));
		}
		else
		{
			_strand->post(std::bind([this](Notify& ntf)
			{
				_unlock(std::forward<Notify>(ntf));
			}, std::forward<Notify>(ntf)));
		}
	}

	const shared_strand& self_strand() const
	{
		return _strand;
	}
private:
	template <typename Handler>
	CoNotifyHandlerFace_* wrap_notify(Handler&& handler)
	{
		assert(_strand->running_in_this_thread());
		typedef CoNotifyHandler_<RM_CREF(Handler)> Handler_;
		return new(_alloc.allocate(sizeof(Handler_)))Handler_(std::forward<Handler>(handler));
	}

	template <typename Notify>
	void _lock(Notify&& ntf)
	{
		assert(_strand->running_in_this_thread());
		if (_locked)
		{
			_waitQueue.push_back(wrap_notify(std::forward<Notify>(ntf)));
		}
		else
		{
			_locked = true;
			CHECK_EXCEPTION(ntf);
		}
	}

	template <typename Notify>
	void _try_lock(Notify&& ntf)
	{
		assert(_strand->running_in_this_thread());
		if (_locked)
		{
			CHECK_EXCEPTION(ntf, co_async_state::co_async_fail);
		}
		else
		{
			_locked = true;
			CHECK_EXCEPTION(ntf, co_async_state::co_async_ok);
		}
	}

	template <typename Notify>
	void _timed_lock(async_timer& timer, int tm, Notify&& ntf)
	{
		assert(_strand->running_in_this_thread());
		if (_locked)
		{
			_waitQueue.push_back(wrap_notify(std::bind([this, &timer](Notify& ntf, co_async_state state)
			{
				timer->cancel();
				CHECK_EXCEPTION(ntf, state);
			}, std::forward<Notify>(ntf), __1)));
			timer->timeout(tm, std::bind([this](const co_notify_node& it)
			{
				CoNotifyHandlerFace_* Ntf = *it;
				_waitQueue.erase(it);
				CoNotifyHandlerFace_::notify_state(_alloc, Ntf, co_async_state::co_async_overtime);
			}, --_waitQueue.end()));
		}
		else
		{
			_locked = true;
			CHECK_EXCEPTION(ntf, co_async_state::co_async_ok);
		}
	}

	template <typename Notify>
	void _unlock(Notify&& ntf)
	{
		assert(_strand->running_in_this_thread());
		assert(_locked);
		if (!_waitQueue.empty())
		{
			CoNotifyHandlerFace_* Ntf = _waitQueue.front();
			_waitQueue.pop_front();
			CoNotifyHandlerFace_::notify_state(_alloc, Ntf);
		}
		else
		{
			_locked = false;
		}
		CHECK_EXCEPTION(ntf);
	}
private:
	shared_strand _strand;
	reusable_mem _alloc;
	msg_list<CoNotifyHandlerFace_*> _waitQueue;
	bool _locked;
	NONE_COPY(co_mutex);
};

#endif