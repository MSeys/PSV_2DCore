#include "Core.h"
#include <psp2/kernel/processmgr.h>

#include <ctime>

int main(int argc, char *argv[]) 
{
	srand(time(nullptr));
	
	Core core;
	core.Run();

	sceKernelExitProcess(0);
	return 0;
}
