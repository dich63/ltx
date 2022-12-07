// ltx_test1.cpp : Defines the entry point for the application.
//

#include "test.h"
#include "ltxjs.h"

#include <map>
#include <mutex>
#include <string>
#include <windows.h>

typedef ipc_utils::com_scriptor_t javascript_t;
typedef ipc_utils::com_dispcaller_t caller_t;
typedef javascript_t::value_t jsvalue_t;
typedef i_mbv_buffer_ptr::int64_t int64_t;
typedef ltx_helper::arguments_t<jsvalue_t>  arguments_t;


#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)  

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
){	
	return TRUE;
}

EXTERN_DLL_EXPORT  void __cdecl  cb_sum
(variant_t & result, pcallback_lib_arguments_t pltx, pcallback_context_arguments_t pcca, int argc, variant_t * argv, int arglast, HRESULT & hr, void*, unsigned flags){
	arguments_t args(pcca);
	double sum=0.0;
	for (auto k = 0; k < args.length(); k++) {

		sum += (double)args[k];
	}

	result = sum;
}