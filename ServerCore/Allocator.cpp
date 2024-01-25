#include "pch.h"
#include "Allocator.h"

/*

  �α׸� ��ų� ī������ �Ҵ��� ��ü ���� Ȯ��
  �޸𸮸��� �Ͼ���� Ȯ���ϱ� ���� �Ҵ�, ���� ¦�� ���� �׽�Ʈ�� ����

*/

/// 
void* BaseAllocator::Alloc(int32 size) {
	return ::malloc(size);
}

void BaseAllocator::Release(void* ptr) {
	::free(ptr);
}




/// 
void* StompAllocator::Alloc(int32 size) {
	const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	const int64 dataOffset = pageCount * PAGE_SIZE - size;
	void* baseAddress = ::VirtualAlloc(nullptr, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	return static_cast<void*>(static_cast<int8*>(baseAddress) + dataOffset);
}

void StompAllocator::Release(void* ptr) {
	const int64 address = reinterpret_cast<int64>(ptr);
	const int64 baseAddress = address - (address % PAGE_SIZE);
	::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
}