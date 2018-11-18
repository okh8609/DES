#include "StrConv.h"

std::string strConv(System::String^ strIn)
{

	const char* chars = (const char*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(strIn)).ToPointer();
	std::string strOut(chars);
	return strOut;
}

System::String^ strConv(std::string strIn)
{
	System::String^ strOut = gcnew System::String(strIn.c_str());
	return strOut;
}
