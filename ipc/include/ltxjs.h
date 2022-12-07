#pragma once

#include "ltx_utils/ipc_utils_base.h"
#include "ltx_utils/dispatch_call_helpers.h"
#include "ltx_utils/typecase.h"
#include "ltx_utils/bindEx.h"
#include "utils/utils.h"
#include "ltx_utils/asyncb.h"

typedef bstr_t olestr_t;
namespace ipc_utils {

	inline bool is_external(smart_ptr_t<IDispatch>& sp) {
		HRESULT hr;
		return  SUCCEEDED(hr = ::CoGetObject(olestr_t("ltx.bind:external"), NULL, __uuidof(IDispatch), sp.ppQI())) ? true : false;
	};
	
	inline bool is_external() {
		COMInitF_t cf;
		smart_ptr_t<IDispatch> sp;
		return is_external(sp);	
	}

	/*!
	 * @brief 
	 * @param dc ipc_utils::dispcaller_t<com_exception>
	 * @return true/false
	*/
	inline bool is_external(com_dispcaller_t& dc) {
		bool f;
		smart_ptr_t<IDispatch> sp;
		if (f = is_external(sp)) {
			if(FAILED(dc.reset(sp.p)))
				return false;
		}
		return f;
	};

	/*!
	 * @brief 
	 * @tparam O is class inherited  of dispatch_by_name_wrapper_t<...> or 
     * dispatch_wrapper_t<...>
	 * @tparam ...Arguments  
	 * @param ...args  -- arguments of constructor O
	 * @return 0
	*/
	template<class O, typename... Arguments>
	int wrap_as_server(const Arguments&... args) {

		COMInitF_t com_initialize;
		typedef com_exception_helper_t<com_exception> exception_helper_t;
		
		com_dispcaller_t external;

		if (is_external(external)) {

			variant_t ec = ipc_utils::bindObject("ltx.bind:external.connector:  global=3 ;")();
			std::pair<variant_t, O*> pp = wrap2<O, com_exception, (FLAG_CBD_EC) << 0>(args...);
			int len = external["length"];
			pp.second->init_params(len, external);
			external["result"] =  pp.first;

			external.reset();

			return loader_ltx_t::get_instance().ltx_external_connector_wait(INFINITE, NULL);
		}
		return -1;

	}

	olestr_t exe_path() {		
		wchar_t buf[4096];
		::GetModuleFileNameW(nullptr, buf, 4096);
		return buf;
	};


}; //namespace ipc_utils
//#include "char_parsers/wchar_parsers.h"