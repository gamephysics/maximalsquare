#pragma once

//=============================================================================
// TYPEDEFS
//=============================================================================
typedef	unsigned long long  UInt64;
typedef   signed long long  Int64;
typedef		  unsigned int	UInt32;
typedef			signed int  Int32;
typedef unsigned short int  UInt16;
typedef   signed short int  Int16;
typedef       unsigned char UInt8;
typedef         signed char SByte;
typedef       unsigned char BYTE;

namespace core
{
	//	std::vector
	template <typename _Ty>	struct is_vector : std::false_type {};
	template <typename _Ty>	struct is_vector<std::vector<_Ty>> : std::true_type {};

};