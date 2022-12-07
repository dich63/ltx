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
*
*
*/
int main()
{
	ipc_utils::COMInitF_t com_initialize;

	ipc_utils::com_dispcaller_t external;

	if (!is_external(external)) {
		::FatalAppExitA(0, "external COM object  of parent process not found");
	}

	double sum = 0.0;
	int length = external["length"];

	//std::cout << "length=" << length << "\n";

	for (auto k = 0; k < length; k++) {
		sum += (double)external[k];
	}

	//std::cout << "sum=" << sum << "\n";

	external["result"] = sum;



return 0;
}
