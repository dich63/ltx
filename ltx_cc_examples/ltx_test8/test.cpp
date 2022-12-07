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

	constexpr auto GB = 1LL << 30;
	//MAKEINTRESOURCE

	auto ss = R"(
function python_def_opts(){   
  function _py(){
     try{
	  //return fileSearch('python.exe',getenv('python_path')+';'+getenv('path'));
	  //return getenv('ltx_js_root')+'py.bat';
	  var s=getenv('ltx_python');
	  return s?s:fileSearch('python.exe');
	 }catch(e){}
	 return ''
  }
)";

	std::cout << ss;
	

	ipc_utils::COMInitF_t com_initialize;

	//auto buffer = createBuffer<double>(GB/ sizeof(double));
	auto buffer = ipc_utils::bindObject("ltx.bind:mm_buffer:len=#1;type=double")(GB / sizeof(double));

	i_mm_region_ptr::ptr_t<double> array(buffer, -1);
	for (auto k = 0; k < array.size(); k++) {
		array[k] = k;		
	}
	

	//auto process_path = ipc_utils::exe_path() + "/../../ltx_test5/ltx_test5.exe";
	auto process_path = "*/../ltx_test5/ltx_test5.exe";
	auto process = ipc_utils::bindObject("ltx.bind:process:file=#1;job=+;cf=16")(process_path);
	double sum;
	stopwatch_t<> sw;
	{
		i_mm_region_ptr::ptr_t<double>::unlocker_t unlock(array);
		
		sum = call(process)(buffer,-10.001);
	}
	std::cout << "\ntimeit: " << sw.sec() << " sec\n"<< std::endl;

	for (auto k = 0; k < 8; k++) {
		std::cout << "array["<<k<<"]="<< array[k] << std::endl;
	}
	std::cout << "...\n...\n";
	for (auto k = array.size()-8; k < array.size(); k++) {
		std::cout << "array[" << k << "]=" << array[k] << std::endl;
	}
	std::cout << "sum=" << sum << std::endl;
	
	return 0;
}
