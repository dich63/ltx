// ltx_test1.cpp : Defines the entry point for the application.
//

#include "test.h"
#include "ltxjs.h"

#include "common.h"


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
	ipc_utils::COMInitF_t com_initialize;

	ipc_utils::com_dispcaller_t external;

	double sum=0.0;

	if (is_external(external)) {
		
			i_mm_region_ptr::ptr_t<double> array((jsvalue_t)external[0]);

			auto& dump30 = pN_t<30>::ref<double>(array);


			double mul = external[1];
			for (auto k = 0; k < array.size(); k++) {

				array[k] *= mul;
				sum += array[k];
			}
		
		//std::cout << "sum=" << sum << "\n";
		external["result"] = sum;
		
	}
	else {
		::FatalAppExitW(0, L"external COM object  of parent process not found");
	}

	
	return 0;
}
