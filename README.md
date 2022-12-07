
### Примеры использования библиотеки ltx_js.

+ **Проекты ltx_test4,ltx_test3:** 

Процесс ltx_test4.exe вызывает  процесс ltx_test3.exe как функцию с переменным числом аргументов.

`ltx_test4.сpp`
``` cpp
int main()
{
	

	ipc_utils::COMInit_t com_initialize(COINIT_MULTITHREADED);
	
	auto process_path =  "*/../ltx_test3/ltx_test3.exe";		
	auto process = ipc_utils::bindObject("ltx.bind:process:file=#1")(process_path);
	
	double sum = ipc_utils::com_dispcaller_t(process)(1.0, 2.0,3.0,4.0,5.0);
	
	std::cout << sum << "\n";
	return 0;
}

```
bindObject создает COM-объект process, вызов которого "call(process)(1.0, 2.0,3.0,4.0,5.0)"
состоит в создании нового процесса "ltx_test3.exe"  и передаче ему ссылки на объект external.
external содержит в себе передаваемые параметры  external[0],external[1],... и результат вызова 
external["result"]. 


`ltx_test3.сpp`
``` cpp
int main()
{	
	ipc_utils::COMInit_t com_initialize(COINIT_MULTITHREADED);
	
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

```

+ **Проекты ltx_test6,ltx_test5:** 

Аналогично предыдущему примеру  пара процессов в которых передается и модифицируется memory map array.
 `ltx_test6.сpp` 
``` cpp
int main()
{
	typedef ipc_utils::com_dispcaller_t call;
	typedef i_mm_region_ptr::ptr_t<double> array_t;

	constexpr auto GB = 1LL << 30;
	
	ipc_utils::COMInitF_t  com_initialize;
	
	

	auto len = GB / sizeof(double);	
	
	auto buffer = ipc_utils::bindObject("ltx.bind:mm_buffer:len=#1;type=#2")(len, type2str<double>());


	array_t array(buffer, -1);


	for (auto k = 0; k < array.size(); k++) {
		array[k] = k;		
	}
	
	auto process_path = "*/../ltx_test5/ltx_test5.exe";
	auto process = ipc_utils::bindObject("ltx.bind:process:file=#1")(process_path);
	
	double sum;
	stopwatch_t<> sw;
	{
		array_t::unlocker_t unlock(array);	
	
		
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


```
`ltx_test5.сpp`
``` cpp
int main()
{	
	ipc_utils::COMInit_t com_initialize(COINIT_MULTITHREADED);
	
	ipc_utils::com_dispcaller_t external;

	if (!is_external(external)) {
		::FatalAppExitA(0, "external COM object  of parent process not found");
	}

	double sum = 0.0;
	int length = external["length"];

	

	for (auto k = 0; k < length; k++) {
		sum += (double)external[k];
	}

	

	external["result"] = sum;
	return 0;
}

```

Обратите внимание на строку `array_t::unlocker_t unlock(array);`. класс  `unlocker_t` снимает блокировку мьютекса
синхронизации памяти на время вызова ltx_test5.exe (иначе клинч)

[//]: # ($$V_{sphere} = \frac{4}{3}\pi r^3$$)

+ **Проекты ltx_test2,ltx_test1:** 

Пример превращения "на лету" процесса ltx_test1.exe в outproc COM сервер (в частности, содержащий словарь объектов - строк чисел и COM-объектов)

`ltx_test2.сpp`
``` cpp
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
```

`ltx_test1.сpp`
``` cpp
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

		::FatalAppExitA(0, "external COM object  of parent process not found");
	}
	
	return err;
}

```

