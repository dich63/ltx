// ltx_test1.cpp : Defines the entry point for the application.
//

#include "test.h"
#include "ltxjs.h"


#include <string>

typedef ipc_utils::com_scriptor_t javascript_t;
typedef ipc_utils::com_dispcaller_t caller_t;
typedef javascript_t::value_t jsvalue_t;
typedef i_mbv_buffer_ptr::int64_t int64_t;
typedef ltx_helper::arguments_t<jsvalue_t>  arguments_t;


//using namespace std;


int main()
{
	ipc_utils::COMInitF_t com_initialize;

	//auto process_path=ipc_utils::exe_path()+ "/../../ltx_test1/ltx_test1.exe";
	auto process_path = "../ltx_test1/ltx_test1.exe";
	
	auto process = ipc_utils::bindObject("ltx.bind:process:file=#1;job=+;cf=16")(process_path);

	auto o= caller_t(process)(1, 2.333, L"message...");

	caller_t r(o);
	r["val"] = "AAA";
	jsvalue_t a0 = r[0];
	jsvalue_t a1 = r[1];
	jsvalue_t a2 = r[2];
	jsvalue_t val = r["val"];

	std::cout << (int)a0<<"\n" << (double)a1 << "\n" << (char*)olestr_t(val) << "\n";

	
	//ipc_utils::interact_js();

	return 0;
}
