// simple_server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ltxjs.h"
//#include "ltx_utils/typecase2.h"
/*!
*  simple_server ltx_test1.exe 
* 
*/


#include <map>
#include <mutex>
#include <string>

typedef ipc_utils::com_scriptor_t javascript_t;
typedef ipc_utils::com_dispcaller_t caller_t;
typedef javascript_t::value_t jsvalue_t;
typedef i_mbv_buffer_ptr::int64_t int64_t;
typedef ltx_helper::arguments_t<jsvalue_t>  arguments_t;


struct ObjectMapExample_t :ltx_helper::dispatch_by_name_wrapper_t< ObjectMapExample_t, jsvalue_t, std::mutex>
{
	struct jsvalue_holder_t
	{
		jsvalue_holder_t() {}
		jsvalue_holder_t(jsvalue_t rv) { v = rv; }
		jsvalue_t v;
		inline operator jsvalue_t() { return v; }
	};

	typedef std::map<std::wstring, jsvalue_holder_t> jsmap_t;
	/*!
	 * @brief 
	 * @param id 
	 * @param name 
	 * @param result 
	 * @param arguments 
	 * @param helper 
	*/
	inline  void  on_get(DISPID id, const wchar_t* name, com_variant_t& result, arguments_t& arguments, i_marshal_helper_t* helper) {

		if (0 == id) {
			std::cout << "clone ObjectMapExample_t ...\n";
			result = ipc_utils::wrap_ec<ObjectMapExample_t>();
			return;
		}
		if (!name)  arguments.raise_error(L"on_get ХряпЦ!!", L"ObjectMapExample_t");
		_cwprintf(L"[%d] on_get %s\n", GetCurrentThreadId(), name);
		result = jsmap[name];

	};

	inline  void  on_put(DISPID id, const wchar_t* name, com_variant_t& value, arguments_t& arguments, i_marshal_helper_t* helper) {

		if (!name)  arguments.raise_error(L"on_put ХряпЦ!!", L"ObjectMapExample_t");
		_cwprintf(L"{%d}[%d] on_put %s\n", ipc_utils::COMInit_t::apartment_type(), GetCurrentThreadId(), name);
		jsmap[name] = jsvalue_holder_t(value);
	};

	template<class Params>
	inline HRESULT init_params(int count, Params& prms) {

		_cwprintf(L"{%d}[%d] init params\n", ipc_utils::COMInit_t::apartment_type(), GetCurrentThreadId());
		for (auto k = 0; k < count; k++) {
			jsmap[(const wchar_t*)olestr_t(jsvalue_t(k))] = prms[k];
		}
		return S_OK;
	}

	ObjectMapExample_t() :dispatch_by_name_wrapper_t() {
		std::cout << "create ObjectMapExample_t ...\n";
	};

	~ObjectMapExample_t() {

		std::cout << "destroying ObjectMapExample_t ...\n";
	}

	jsmap_t jsmap;

};



int main()
{
	ipc_utils::COMInitA_t com_initialize;
	long err=0;
	if (ipc_utils::is_external()) {

		err = ipc_utils::wrap_as_server<ObjectMapExample_t>();
		if (err) {
			std::cout << "server stoped ...\n";
			::SleepEx(2000, true);
		}
	}
	else {

		auto p2 = ipc_utils::wrap2<ObjectMapExample_t, ipc_utils::com_exception,0>();

		auto o = p2.first;
		auto p = p2.second;
		p->jsmap[L"aa"] = ObjectMapExample_t::jsvalue_holder_t("AAAA");
		caller_t js("ltx.bind:script");
		js("o = $$[0]", o);
		ipc_utils::interact_js(js);
	}
	
	return err;
}

