#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]

void main()
{
	//建立任何控制項之前，先啟用 Windows XP 視覺效果
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	//建立主視窗並執行
	Application::Run(gcnew DES::MyForm());
}