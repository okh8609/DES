#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]

void main()
{
	//�إߥ��󱱨���e�A���ҥ� Windows XP ��ı�ĪG
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	//�إߥD�����ð���
	Application::Run(gcnew DES::MyForm());
}