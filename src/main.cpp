#include "Core.h"

#include <psp2/kernel/processmgr.h>

int main(int argc, char *argv[]) 
{
	Core core;
	core.Run();

	sceKernelExitProcess(0);
	return 0;
}
