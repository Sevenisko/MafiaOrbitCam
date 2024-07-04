#include "C_chunk.h"
//#include <rw_data.h>

bool C_chunk::Read(void* pBuffer, size_t uSize) {
	//if(!m_bIsOpen || m_bIsWritable) {
	//	return false;
	//}

	//if ( !m_uFileBegin ) {
	//	return dtaRead(m_uFileHandle, (char*)pBuffer, (int32_t)uSize) >= (int32_t)uSize;
	//}

	//uint32_t sizeUnk = (this->m_uFileEnd - m_uFileBegin) >> 2;
	//if ( !sizeUnk ) {
	//	return dtaRead(this->m_uFileHandle, (char*)pBuffer, (int32_t)uSize) >= (int32_t)uSize;
	//}
	//
	//int iPos = dtaSeek(m_uFileHandle, 0, 1);
	//uint32_t uOffset = m_uFileBegin - iPos;
	//
	//return uOffset <= uSize && dtaRead(m_uFileHandle, (char*)pBuffer, (int32_t)uSize) >= (int32_t)uSize;

	
	return ((bool(__thiscall*)(C_chunk*, void*, size_t))(0x00404D80))(this, pBuffer, uSize);
}