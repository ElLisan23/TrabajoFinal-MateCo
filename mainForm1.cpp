#include "mainForm1.h"

using namespace System;
using namespace System::Windows::Forms;

int main(cli::array<System::String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Mostrar pantalla de presentación primero
    TrabajoFinalMateComputacionalAlgoritmoDijkstra2::mainForm inicio;
    Application::Run(% inicio);
    return 0;
}