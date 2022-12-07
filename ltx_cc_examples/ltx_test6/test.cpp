// ltx_test1.cpp : Defines the entry point for the application.
//

//#include "test.h"
#include "ltxjs.h"

#include <map>
#include <mutex>
#include <string>
#include "common.h"



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
	typedef i_mm_region_ptr::ptr_t<double> array_t;

	constexpr auto GB = 1LL << 30;
	
	//ipc_utils::COMInitF_t  com_initialize;
	COMInitF;
	

	auto len = GB / sizeof(double);
	
	//auto buffer = ipc_utils::bindObject("ltx.bind:mm_buffer:len=#1;type=double")(len);
	auto buffer = ipc_utils::bindObject("ltx.bind:mm_buffer:len=#1;type=#2")(len, type2str<double>());


	array_t array(buffer, -1);

	auto& dump16 = pN_t<16>::ref<double>(array);

	for (auto k = 0; k < array.size(); k++) {
		array[k] = k;		
	}
	

	//auto process_path = ipc_utils::exe_path() + "/../../ltx_test5/ltx_test5.exe";
	auto process_path = "*/../ltx_test5/ltx_test5.exe";
	auto process = ipc_utils::bindObject("ltx.bind:process:file=#1;cf=16;dbghook=1")(process_path);
	double sum;
	stopwatch_t<> sw;
	{
		array_t::unlocker_t unlock(array);	
		//UNNAME_DECLARE(array_t::unlocker_t)(array);
		
		sum = call(process)(buffer,-10.001);
		auto t = sw.sec();
		std::cout << "\ntimeit: " << t << " sec\n" << std::endl << std::flush;
	}

	

	for (auto k = 0; k < 8; k++) {
		std::cout << "array["<<k<<"]="<< array[k] << std::endl << std::flush;;
	}
	std::cout << "...\n...\n";
	for (auto k = array.size()-8; k < array.size(); k++) {
		std::cout << "array[" << k << "]=" << array[k] << std::endl << std::flush;
	}

	std::cout << "sum=" << sum << std::endl << std::flush;;
	
	return 0;
}
