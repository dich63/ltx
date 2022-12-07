#pragma once
#include "utils/utils.h"
#include "ltx_utils/ipc_utils_base.h"
#include "dispatch_call_helpers.h"

namespace ipc_utils {

	inline HRESULT get_ko_handle(variant_t vo,HANDLE* phko) {

		HRESULT hr;
		
		smart_ptr_t<i_mbv_kernel_object> ko;

		if (SUCCEEDED(hr = ko.reset(vo))) {

			pkernel_object_t pko = nullptr;
			if (SUCCEEDED(hr = ko->GetContext((void**)&pko))) {
				if (phko) *phko = pko->hko;
			}
		}

		return hr;
	}

	template< class ExceptionClass>
	struct event_asyn_t {

		typedef variant_t jsvalue_t;
		typedef com_exception_helper_t<ExceptionClass > exception_helper_t;

		typedef dispcaller_t<ExceptionClass> caller_t;

		event_asyn_t(HANDLE _handle, variant_t _o):handle(_handle),o(_o) {
		}

		//template <class Fun>
		//wait()
		template<class FUN>
		jsvalue_t result_wait(const FUN& fun, DOUBLE tow = 100) {
			DWORD ws;
			while (WAIT_TIMEOUT == (ws = WaitForSingleObject(handle, tow))) {
				fun(cs.sec());
			}
			if (ws != WAIT_OBJECT_0) {
				exception_helper_t::check_error(E_FAIL);
			};

			caller_t caller(o);
			int err = caller["error"];

			exception_helper_t::check_error(err);

			return caller["result"];

			
		}

		template<class FUN>
		jsvalue_t operator()(const FUN& fun, DOUBLE tow = 100) {
			return result_wait(fun, tow);
		}
		utils::stopwatch_t<0> cs;
		HANDLE handle;
		variant_t o;
	};

	template< class ExceptionClass = com_exception>
	struct asyn_call_t :invoker_base_t<asyn_call_t<ExceptionClass >, variant_t, event_asyn_t<ExceptionClass>,VARIANT> {
		typedef ExceptionClass  exception_t;
		typedef com_exception_helper_t<ExceptionClass > exception_helper_t;
		typedef variant_t jsvalue_t;
		typedef dispcaller_t<exception_t> caller_t;

		
		asyn_call_t(jsvalue_t func) :innerjs("ltx.bind:script"){
			
			innerjs("var asyn=bindObject('asyn:'),func=$$[0]",func);                
        
		}

		inline 	event_asyn_t<ExceptionClass> invoke_proc(int argc, VARIANT* argv) {
			HANDLE handle = nullptr;
			jsvalue_t vo=innerjs("o=asyn(func)");
			//caller_t o = vo;			
			caller_t(vo).invoke_proc(argc, argv);
			  

			handle = (HANDLE) int64_t( double(innerjs("asyn_event=o.event;asyn_event.handle")));

			

			return event_asyn_t<ExceptionClass>(handle,vo);
		}

		HRESULT hr;
		//jsvalue_t result;
		jsvalue_t o;
		caller_t innerjs;
		//smart_ptr_t<i_mbv_kernel_object> mbv_event;
	};



	template< class ExceptionClass>
	struct event_asyn_script_t {

		typedef variant_t jsvalue_t;
		typedef com_exception_helper_t<ExceptionClass > exception_helper_t;

		typedef dispcaller_t<ExceptionClass> caller_t;

		event_asyn_script_t(HANDLE _handle, jsvalue_t _o) :handle(_handle), o(_o) {

		}

		//template <class Fun>
		//wait()
		template<class FUN>
		jsvalue_t result_wait(FUN fun, DOUBLE tow = 100) {
			DWORD ws;
			HRESULT hr;
			while (WAIT_TIMEOUT == (ws = WaitForSingleObject(handle, tow))) {
				fun(cs.sec());
			}
			if (ws != WAIT_OBJECT_0) {
				exception_helper_t::check_error(hr=E_FAIL);
			};

			caller_t caller(o);

			hr =(int) caller["error"];			

			exception_helper_t::check_error(hr,"inner asyn error");
			
			caller_t result = (jsvalue_t)caller["result"];

			hr = (int)result["error"];
			if (FAILED(hr)) {

				jsvalue_t vmessage =result["message"];

				exception_helper_t::check_error(hr, (wchar_t*)bstr_t(vmessage));
			}
			
			
			return result["result"];


		}

		template<class FUN>
		jsvalue_t operator()(FUN fun, DOUBLE tow = 100) {
			return result_wait(fun, tow);
		}
		utils::stopwatch_t<0> cs;
		HANDLE handle;
		variant_t o;
	};




	template< class ExceptionClass = com_exception>
	struct asyn_script_t :invoker_base_t<asyn_script_t<ExceptionClass >, variant_t, event_asyn_script_t<ExceptionClass>, VARIANT>
	{
		typedef ExceptionClass  exception_t;
		typedef com_exception_helper_t<ExceptionClass > exception_helper_t;
		typedef variant_t jsvalue_t;
		typedef dispcaller_t<exception_t> caller_t;

		asyn_script_t(jsvalue_t _script,bstr_t moniker= L"ltx.bind:script")
			: innerjs(moniker)
		{
			disp_funct=caller_t(_script)("require('asyn').safe_globalEval");
			script.reset(_script);		    
		}

		asyn_script_t(caller_t& _script, bstr_t moniker = L"ltx.bind:script")
			: innerjs(moniker)
    	 {
			disp_funct = _script("require('asyn').safe_globalEval");
			script.reset(_script.disp.p);
		}

		inline 	event_asyn_script_t<ExceptionClass> invoke_proc(int argc, VARIANT* argv) {
			HANDLE handle = nullptr;
			jsvalue_t vo = innerjs("bindObject('asyn:')($$[0])", disp_funct );
			//caller_t o = vo;			
			caller_t(vo).invoke_proc(argc, argv);


			//handle = (HANDLE)int64_t(double(innerjs("asyn_event=o.event;asyn_event.handle")));

			vhandle = innerjs("$$[0].event",vo);

			exception_helper_t::check_error(hr = get_ko_handle(vhandle, &handle));			
			
			return event_asyn_script_t<ExceptionClass>(handle, vo);
		}

		
		HRESULT hr;
		jsvalue_t disp_funct;
		//jsvalue_t o,result;
		jsvalue_t vhandle;
		ipc_utils::smart_ptr_t<IDispatch> script;		
		caller_t innerjs;
		//asyn_call_t<ExceptionClass> asyn_call;
		//smart_ptr_t<i_mbv_kernel_object> mbv_event;
	};


	/*
	template< class ExceptionClass = com_exception>
	struct asyn_run_t :invoker_base_t<asyn_run_t<ExceptionClass >, variant_t, event_asyn_script_t<ExceptionClass>, VARIANT> {
		typedef ExceptionClass  exception_t;
		typedef com_exception_helper_t<ExceptionClass > exception_helper_t;
		typedef variant_t jsvalue_t;
		typedef dispcaller_t<exception_t> caller_t;

		template<class Func,class Caller>
		asyn_run_t(Caller& caller,Func func) {};

		asyn_script_t<ExceptionClass> asyn_script;
	};
	*/

            


};// end namespace ipc_utils