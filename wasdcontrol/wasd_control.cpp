#include <iostream>
#include <string>
#include <cstdlib>
#include <barrett/units.h>
#include <barrett/systems.h>
#include <barrett/products/product_manager.h>
#include <barrett/standard_main_function.h>

using namespace barrett;

template<size_t DOF>
int wam_main(int argc, char** argv, ProductManager& pm, systems::Wam<DOF>& wam)
{
	BARRETT_UNITS_TEMPLATE_TYPEDEFS(DOF);

	std::string line;
	jp_type prev_jp = wam.getJointPositions();
	jp_type move_jp;
	bool active = true;

	wam.gravityCompensate();

	while (active)
	{
		for (int i = 0; i < move_jp.size(); i++)
			move_jp[i] = 0;

		while(!wam.moveIsDone()); // Is moveTo non-blocking? 

		std::getline(std::cin, line);

		switch (line[0]) {
		case 'a':
			move_jp[0] = -0.1;
			break;
		case 'd':
			move_jp[0] = 0.1;
			break;
		case 'w':
			move_jp[1] = 0.1;
			break;
		case 's':
			move_jp[1] = -0.1;
			break;
		case 'h':
			move_jp[2] = -0.1;
			break;
		case 'k':
			move_jp[2] = 0.1;
			break;
		case 'u':
			move_jp[3] = 0.1;
			break;
		case 'j':
			move_jp[3] = -0.1;
			break;
		case 'q':
			active = false;
			wam.moveHome()
			break;
		}

		prev_jp = prev_jp + move_jp;
		wam.moveTo(prev_jp);
	}

	wam.idle();
	pm.getSafetyModule()->waitForMode(SafetyModule::IDLE);
	return 0;
}

