#include "stdafx.h"
#include "Application.h"

int main(int argc, char* argv[])
{
    Application* context = Application::instance();
    return context->exec(argc, argv);
}
