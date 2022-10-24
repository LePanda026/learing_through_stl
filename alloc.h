#ifndef ALLOC
#define ALLOC

#include<cstdlib>

namespace std {

	class alloc{
		//�����Ķ���
	private:
		enum EAlign { ALIGN = 8 };//С��������ϵ��߽�
		enum EMaxBytes { MAXBYTES = 128 };//С������ĸ��������ޣ�������������malloc����
		enum ENFreeLists { NFREELISTS = EMaxBytes::MAXBYTES / EAlign::ALIGN };//free_lists�ĸ���
	//free_lists
	private:
		//free_lists�Ľڵ㹹��
		union obj {
			union obj* next;
			char client_data[1];
		};

		//free_list����
		static obj* free_lists[ENFreeLists::NFREELISTS];

		//�ڴ��
	private:
		static char* start_free;
		static char* end_free;
		static size_t heap_size;

		//��free_lists���в����ĺ���
		static size_t RoundUp(size_t bytes) {
			return ((bytes + EAlign::ALIGN - 1) & ~(EAlign::ALIGN - 1));
		}

		static size_t Index_Freelist(size_t bytes) {
			return ((bytes + EAlign::ALIGN - 1) / EAlign::ALIGN - 1);
		}

		static void* refill(size_t n);

		static char* chunk_alloc(size_t size, int & nobjs);

	public:
		static void* allocate(size_t bytes);
		static void deallocate(void* ptr, size_t bytes);
		static void* reallocate(void* ptr, size_t size_old, size_t size_new);
	};
}
#endif // !ALLOC