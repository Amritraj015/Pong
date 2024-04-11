#include "engine.h"
#include "defines.h"

int main()
{
	auto application = std::make_unique<Engine::Application>(create_application());

    return 0;
}