// ltx_test1.cpp : Defines the entry point for the application.
//

#include "test.h"
#include "ltxjs.h"

#include <map>
#include <mutex>
#include <string>
typedef ipc_utils::com_scriptor_t javascript_t;
typedef ipc_utils::com_dispcaller_t caller_t;
typedef javascript_t::value_t jsvalue_t;
typedef i_mbv_buffer_ptr::int64_t int64_t;
typedef ltx_helper::arguments_t<jsvalue_t>  arguments_t;


//using namespace std;

/*!
 * @brief  
 * Process as a function: calculate the sum of the input arguments
 * @return 
*/
int main()
{
	typedef ipc_utils::com_dispcaller_t call;
	

	ipc_utils::COMInitF_t com_initialize;
	
	auto process_path =  "*/../ltx_test3/ltx_test3.exe";
	
	auto process = ipc_utils::bindObject("ltx.bind:process:file=#1;job=+;cf=16")(process_path);

	double sum = call(process)(1.0, 2.0,3.0,4.0,5.0);

	std::cout << sum << "\n";
	return 0;
}
