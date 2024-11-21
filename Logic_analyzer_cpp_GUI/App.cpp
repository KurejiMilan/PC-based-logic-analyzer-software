#include "App.h"
#include "Main.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() 
{
	MainFrame* frame = new MainFrame(wxT("Logic Analyzer"));
	frame->Maximize();
	frame->Show(true);
	return true;
}