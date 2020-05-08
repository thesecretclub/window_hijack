#ifndef WNDHIJACK_WINDOWS_DEFS_HPP
#define WNDHIJACK_WINDOWS_DEFS_HPP

#include <ntifs.h>
#include <stdint.h>

namespace nt
{
	// NT/Rtl structures
	struct rtl_module_info
	{
		HANDLE section;
		uint64_t mapped_base;
		uint64_t image_base;
		uint32_t image_size;
		uint32_t image_flags;
		uint16_t load_order_idx;
		uint16_t init_order_idx;
		uint16_t load_count;
		uint16_t file_name_offset;
		uint8_t full_path[ 256 ];
	};

	struct rtl_modules
	{
		uint32_t count;
		rtl_module_info modules[ 1 ];
	};

	// PE structures
	struct image_file_header
	{
		uint16_t machine;
		uint16_t number_of_sections;
		uint32_t time_date_stamp;
		uint32_t pointer_to_symbol_table;
		uint32_t number_of_symbols;
		uint16_t size_of_optional_header;
		uint16_t characteristics;
	};

	struct image_data_directory
	{
		uint32_t virtual_address;
		uint32_t size;
	};

	struct image_optional_header
	{
		uint16_t magic;
		uint8_t major_linker_version;
		uint8_t minor_linker_version;
		uint32_t size_of_code;
		uint32_t size_of_initialized_data;
		uint32_t size_of_uninitialized_data;
		uint32_t address_of_entry_point;
		uint32_t base_of_code;
		uint64_t image_base;
		uint32_t section_alignment;
		uint32_t file_alignment;
		uint16_t major_operating_system_version;
		uint16_t minor_operating_system_version;
		uint16_t major_image_version;
		uint16_t minor_image_version;
		uint16_t major_subsystem_version;
		uint16_t minor_subsystem_version;
		uint32_t win32_version_value;
		uint32_t size_of_image;
		uint32_t size_of_headers;
		uint32_t check_sum;
		uint16_t subsystem;
		uint16_t dll_characteristics;
		uint64_t size_of_stack_reserve;
		uint64_t size_of_stack_commit;
		uint64_t size_of_heap_reserve;
		uint64_t size_of_heap_commit;
		uint32_t loader_flags;
		uint32_t number_of_rva_and_sizes;
		image_data_directory data_directory[ 16 ];
	};

	struct image_nt_headers
	{
		uint32_t signature;
		image_file_header file_header;
		image_optional_header optional_header;
	};

	struct image_dos_header
	{
		uint16_t e_magic;
		uint16_t e_cblp;
		uint16_t e_cp;
		uint16_t e_crlc;
		uint16_t e_cparhdr;
		uint16_t e_minalloc;
		uint16_t e_maxalloc;
		uint16_t e_ss;
		uint16_t e_sp;
		uint16_t e_csum;
		uint16_t e_ip;
		uint16_t e_cs;
		uint16_t e_lfarlc;
		uint16_t e_ovno;
		uint16_t e_res[ 4 ];
		uint16_t e_oemid;
		uint16_t e_oeminfo;
		uint16_t e_res2[ 10 ];
		int32_t e_lfanew;
	};

	// Win32k structures
	struct tag_thread_info
	{
		PETHREAD owning_thread;
	};

	struct tag_wnd
	{
		char pad_0[ 0x10 ];
		tag_thread_info* thread_info;
	};
}

#endif