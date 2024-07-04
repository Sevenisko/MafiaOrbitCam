#pragma once
#include "common.h"

class C_chunk {
public:
	//NOTE(DavoSK): this is made up :)
	template<typename T>
	bool Read(T* target) {
		return Read((void*)target, sizeof(T*));
	}

	bool Read(void* pBuffer, size_t uSize);
private:
	uint8_t pad[4];
	uint32_t m_uFileBegin;
	uint32_t m_uFileEnd;
	uint8_t pad2[4];
	bool m_bIsOpen;
	uint8_t pad3[3];
	uint32_t m_uFileHandle;
	bool m_bIsWritable;
	uint8_t pad4[3];
	uint32_t m_uCurrentOffset;
};