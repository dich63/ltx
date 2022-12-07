#pragma once

#include "ltxjs.h"
#include <atomic>

namespace mm_frames {

	constexpr auto TB = (1LL << 40);
	const int64_t i64sign = *((int64_t*)"MapFrame");

	
	struct _initer_t {		
		ipc_utils::COMInitF_t cif;		
	};

	template<class _FRAME>
	struct base_op_t {

		struct _header_t {
			int64_t signature;
			int64_t cursor;
			int64_t frame_size;
			int64_t byte_size;
			int64_t offset;
			char info[0];
		};

		typedef ipc_utils::com_dispcaller_t caller_t;
		typedef _FRAME frame_t;
		

		static HRESULT check_error(HRESULT hr) {
			return ipc_utils::com_dispcaller_t::exception_helper_t::check_error(hr);
		}

		char* info() {
			return mm_header->info;
		}

		std::atomic<int64_t>& cursor_ref() {
			auto pcursor = (std::atomic<int64_t>*) & mm_header->cursor;
			return *pcursor;
		}

		base_op_t(variant_t o, int64_t offset):
			mm_header(o,0, offset)
			//,stub(ipc_utils::bindObject("ltx.bind:stub")())
		{
			check_error(mm_header.hr);

		}

		frame_t* operator [](int64_t i) {
			HRESULT hr;
			auto pt = mm_header.intf();
			auto frame_size = mm_header->frame_size;
			auto offset = mm_header->offset;

			check_error(hr = pt->DecommitRegionPtr(frame_ptr));
			check_error(hr = pt->CommitRegionPtr(offset + i * frame_size, frame_size, (void**)&frame_ptr));

			return frame_ptr;

		}

		inline _header_t* operator ->() {
			return mm_header.ptr;
		}

		char* objref() {
			if (!_ref.length()) {
				stub.reset("ltx.bind:stub");
				_ref = stub(this->mm_header.intf());
			}
			return _ref;
		}

		

		
		i_mm_region_ptr::ptr_t<_header_t> mm_header;
		frame_t* frame_ptr;

		olestr_t _ref;
		caller_t stub;

	};


	template<class _FRAME>
	struct writer_t :_initer_t, base_op_t<_FRAME> {
		typedef _FRAME frame_t;
		

		writer_t(olestr_t filename,int ftemp=1,int64_t frame_size = sizeof(frame_t), int64_t byte_size = TB, int64_t offset = 4096) :
			_initer_t(),
			base_op_t<_FRAME>(
				ipc_utils::bindObject("ltx.bind:mm_buffer:len=#1;name=#2;flags.sparse=1;flags.unsafe=1;flags.temp=#3;")(
					byte_size,
					filename,
					ftemp
					),
				4096)
			{
			
			this->mm_header->cursor = 0;
			this->mm_header->frame_size = frame_size;
			this->mm_header->byte_size = byte_size;
			this->mm_header->offset = offset;
			this->mm_header->signature = i64sign;			
		
			//_ref=stub(this->mm_header.intf());
		}

		writer_t(int64_t frame_size = sizeof(frame_t), int64_t byte_size = TB, int64_t offset = 4096):
			writer_t(olestr_t(), 1, frame_size, byte_size, offset){
		}

		

		frame_t* frame_end() {
			return (*this)[this->cursor_ref()];
		}

		std::atomic<int64_t>& cursor() {
			
			return this->cursor_ref();
		}



	};

	template<class _FRAME>
	struct reader_t :_initer_t,base_op_t<_FRAME> {
		typedef _FRAME frame_t;
		//typedef ipc_utils::com_dispcaller_t caller_t;

		template<class _STR>
		reader_t(_STR objref) :
			_initer_t(),
			base_op_t<_FRAME>(ipc_utils::bindObject(objref)(), 4096) {

			this->check_error(this->mm_header.hr);
			if (i64sign != this->mm_header->signature) {
				this->check_error(E_INVALIDARG);
			}
		}

		int64_t cursor() {

			return this->cursor_ref();
		}

		frame_t* frame_end() {
			return (*this)[cursor()];
		}
		

	};	


} //namespace mm_frames




