#include "types.h"
#include "result.h"
#include "os.h"
#include "svc.h"

#define TICKS_PER_USEC 268.111856
#define TICKS_PER_MSEC 268111.856

//---------------------------------------------------------------------------------
u32 osConvertVirtToPhys(const void* addr) {
//---------------------------------------------------------------------------------
	u32 vaddr = (u32)addr;
	if(vaddr >= 0x14000000 && vaddr < 0x1c000000)
		return vaddr + 0x0c000000; // LINEAR heap
	if(vaddr >= 0x1F000000 && vaddr < 0x1F600000)
		return vaddr - 0x07000000; // VRAM
	if(vaddr >= 0x1FF00000 && vaddr < 0x1FF80000)
		return vaddr + 0x00000000; // DSP memory
	if(vaddr >= 0x30000000 && vaddr < 0x40000000)
		return vaddr - 0x10000000; // Only available under FIRM v8+ for certain processes.
	return 0;
}

//---------------------------------------------------------------------------------
void* osConvertOldLINEARMemToNew(const void* addr) {
//---------------------------------------------------------------------------------
	u32 vaddr = (u32)addr;
	if(vaddr >= 0x30000000 && vaddr < 0x40000000)return (void*)vaddr;
	if(vaddr >= 0x14000000 && vaddr < 0x1c000000)return (void*)(vaddr+0x1c000000);
	return 0;
}

s64 osGetMemRegionUsed(MemRegion region) {
	s64 mem_used;
	svcGetSystemInfo(&mem_used, 0, region);
	return mem_used;
}

//---------------------------------------------------------------------------------
const char* osStrError(u32 error) {
//---------------------------------------------------------------------------------
	switch((error>>26) & 0x3F) {
	case 0:
		return "Success.";
	case 1:
		return "Nothing happened.";
	case 2:
		return "Would block.";
	case 3:
		return "Not enough resources.";
	case 4:
		return "Not found.";
	case 5:
		return "Invalid state.";
	case 6:
		return "Unsupported.";
	case 7:
		return "Invalid argument.";
	case 8:
		return "Wrong argument.";
	case 9:
		return "Interrupted.";
	case 10:
		return "Internal error.";
	default:
		return "Unknown.";
	}
}