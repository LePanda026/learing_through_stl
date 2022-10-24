#include "alloc.h"
#include "malloc_alloc.h"


namespace std {
	char* alloc::start_free = 0;
	char* alloc::end_free = 0;
	size_t alloc::heap_size = 0;

	alloc::obj* alloc::free_lists[alloc::ENFreeLists::NFREELISTS] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};

	void* alloc::refill(size_t n) {
		int nobjs = 20;
		char* chunk = chunk_alloc(n, nobjs);//为啥原来要使用enum设定常量？

		if (1 == nobjs)
			return chunk;
		
		obj* volatile* my_free_list = free_lists + Index_Freelist(n);
		obj* result;
		obj* current_obj;
		obj* next_obj;
		int i;

		result = (obj*)chunk;
		*my_free_list = next_obj = (obj*)(chunk + n);

		for (i = 1; ; i++) {

			current_obj = next_obj;
			next_obj = (obj*)((char*)next_obj + n);
			if (i == nobjs - 1) {
				current_obj->next = 0;
				break;
			}
			else {
				current_obj->next = next_obj;
			}
		}

		return (result);
	}

	void* alloc::allocate(size_t bytes) {
		if (bytes > EMaxBytes::MAXBYTES)
			return malloc_alloc::allocate(bytes);

		obj* volatile *my_free_list;
		obj* result;
		
		my_free_list = free_lists + Index_Freelist(bytes);
		result = *my_free_list;

		if (0 == result) {
			void* refill_place = refill(bytes);
			return refill_place;
		}

		*my_free_list = result->next;
		return result;
	}

	void alloc::deallocate(void* ptr, size_t bytes) {
		if (bytes > EMaxBytes::MAXBYTES)
			malloc_alloc::deallocate(ptr,bytes);
		
		obj* q = (obj*)ptr;
		obj* volatile* my_free_list = free_lists + RoundUp(bytes);
		q->next = *my_free_list;
		*my_free_list = q;
	}

	void* alloc::reallocate(void* ptr, size_t size_old, size_t size_new) {
		deallocate(ptr, size_old);
		ptr = allocate(size_new);
		return ptr;
	}



	
}