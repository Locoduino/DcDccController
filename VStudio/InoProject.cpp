#include "Arduino.h"
#include "DIO2.h"
#include "LcdUi.h"

#include "../examples/Demo/Demo.ino"

//#include "TestExe.ino"

using namespace System;
using namespace System::Windows::Forms;

void OnMouseDown(System::Object ^sender, System::Windows::Forms::MouseEventArgs ^e);

[STAThread]
void Main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	ArduiEmulator::ArduinoForm form(setup, loop);
	form.buttonStart->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(&OnMouseDown);
		
	ArduiEmulator::Arduino::_pinSetup();
	Application::Run(%form);
}

void OnMouseDown(System::Object ^sender, System::Windows::Forms::MouseEventArgs ^e)
{
	ArduiEmulator::Arduino::_pinSetup();
}
