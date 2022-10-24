#include "malloc_alloc.h"

namespace std {
	static void(*malloc_alloc_oom_handler)() = 0;

	void* malloc_alloc::oom_malloc(size_t n) {
		void(*my_malloc_handler)();
		void* result;

		for (;;) {
			my_malloc_handler = malloc_alloc_oom_handler;

			if (0 == my_malloc_handler)
				THROW_BAD_ALLOC;

			(*my_malloc_handler)();

			result = malloc(n);
			if (result)
				return result;
		}

	}

	void* malloc_alloc::oom_realloc(void* p, size_t n) {
		void(*my_malloc_handler)();
		void* result;

		for (;;) {
			my_malloc_handler = malloc_alloc_oom_handler;

			if (0 == my_malloc_handler)
				THROW_BAD_ALLOC;

			(*my_malloc_handler)();
			result = realloc(p, n);
			if (result)
				return result;
		}
	}

	void* malloc_alloc::allocate(size_t n) {
		void* result = malloc(n);
		if (0 == result)
			oom_malloc(n);
	}

	void malloc_alloc::deallocate(void* p, size_t n) {
		free(p);
	}

	void* malloc_alloc::reallocate(void* p, size_t size_old, size_t size_new) {
		void* result = realloc(p, size_new);
		if (0 == result)
			result = oom_realloc(p, size_new);
		return result;
	}

	void(*malloc_alloc::set_malloc_handler(void(*f)()))(){
		void (*old)() = malloc_alloc_oom_handler;
		malloc_alloc_oom_handler = f;
		return (old);
	}
}
