#include "pch.h"
#include "memory.h"
#include "CheatFunction.h"
#include <dwmapi.h>
#include "Offsets.h"

uint64_t VisiblePlayerList;
size_t gameAssemblyGlobal;

bool testBITD(uint32_t Value, uint32_t BitPosition) {
	return (Value & (1 << BitPosition)) != 0;
}

uintptr_t decryptIl2cppHandle(uint32_t ptr) {
	// IDA -> Strings -> "il2cpp_gchandle_get_target" -> F5 -> il2cpp_gchandle_get_target_0
	uint64_t rdi_1 = ptr >> 3;
	uint64_t rcx_1 = (ptr & 7) - 1;
	uint64_t baseAddr = gameAssemblyGlobal + 0xDEDA540 + 0x28 * rcx_1;
	uint32_t limit = TargetProcess.Read<uint32_t>(baseAddr + 0x10);
	if (rdi_1 < limit) {
		uint64_t objAddr = TargetProcess.Read<uint64_t>(baseAddr);
		uint64_t bitMask = TargetProcess.Read<uint64_t>(objAddr + ((rdi_1 >> 5) << 2));
		if (testBITD(bitMask, rdi_1 & 0x1f)) {
			uint64_t objectArray = TargetProcess.Read<uint64_t>(baseAddr + 0x8) + (rdi_1 << 3);
			return TargetProcess.Read<BYTE>(baseAddr + 0x14) > 1
				? TargetProcess.Read<uint64_t>(objectArray)
				: ~TargetProcess.Read<uint64_t>(objectArray);
		}
	}
	return 0;
}

uint64_t networkable_key(uint64_t wrapper)
{

	int* v5; // rdx
	int v6; // r8d
	int v7; // eax
	signed __int64 v8; // rcx
	__int64 target_0; // rax
	__int64 v10; // rcx
	__int64 v11; // rbp
	__int64 v12; // rdi
	__int64 v13; // rsi
	int v14; // eax
	signed __int64 v15; // rcx
	__int64 v17; // rax
	__int64 v18; // rbx
	__int64 v19; // rax
	__int64 v20; // rax
	signed __int32 v21[8]; // [rsp+0h] [rbp-48h] BYREF
	uint64_t v22; // [rsp+20h] [rbp-28h] BYREF
	signed __int64 v23; // [rsp+50h] [rbp+8h] BYREF
	v5 = (int*)&v22;
	v6 = 2;
	v22 = TargetProcess.Read<uint32_t>(wrapper + 0x18);
	do
	{
		v7 = *v5++;
		*(v5 - 1) = (((v7 - 1170066891) << 25) | ((unsigned int)(v7 - 1170066891) >> 7)) ^ 0xAFC48340;
		--v6;
	} while (v6);
	v8 = v22;
	return decryptIl2cppHandle(v8);
}


uintptr_t Ulong(uintptr_t Value)
{

	int* v4;
	int v5;
	unsigned int v6;
	int v7;
	uint64_t v24;

	v4 = (int*)&v24;
	v5 = 2;
	memcpy(&v24, &Value, sizeof(Value));
	do {
		v6 = *v4;
		v7 = *v4++;
		*(v4 - 1) = (((v7 - 601257353) ^ 0x153ACEBD) << 11) | (((v7 - 601257353) ^ 0x153ACEBDu) >> 21);
		--v5;
	} while (v5);
	return v24;
}

uint64_t decrypt_uint32(uint64_t a1) {

	int* v4; // rdxw
	int v5; // r8d
	unsigned int v6; // ecx
	int v7; // eax
	uint64_t v8; // rcx
	uint32_t v24; // [rsp+20h] [rbp-28h] BYREF
	v4 = (int*)&v24;
	v5 = 2;
	v24 = TargetProcess.Read<uint32_t>(a1 + 24);
	do
	{
		v6 = *v4;
		v7 = *v4++;
		*((int*)v4 - 1) = ((v5 - 1094394207) ^ 0x23062206) - 168543866; //
		--v5;
	} while (v5);
	v8 = v24;

	return v8;
}


void writeAdminESP(uint64_t address) {
	uint64_t OcclusionCullingklass = TargetProcess.Read<uint64_t>(gameAssemblyGlobal + occlusioncullingOffset);
	uint64_t OcclusionCullingStaticKlass = TargetProcess.Read<uint64_t>(OcclusionCullingklass + 0xb8);
	printf("OcclusionCullingStaticKlass: %llx\n", OcclusionCullingStaticKlass);
	uint64_t OcclusionCullinginstance = TargetProcess.Read<uint64_t>(OcclusionCullingStaticKlass + 0x248);
	uint64_t debugsettings = TargetProcess.Read<uint64_t>(OcclusionCullinginstance + 0x1A8);
	uint64_t debugValue = TargetProcess.Read<uint64_t>(debugsettings + 0x1c);
	TargetProcess.Write<int>(address + 0x1c, 1);
	TargetProcess.Write<int>(address + 0x20, 131072);
}

void main() {
	setlocale(LC_ALL, "Russian");
	int trying = 1;
	while (!TargetProcess.Init("RustClient.exe")) {
		printf("[0xGOIDA] Что-то пошло не так, чит не был инициализирован, повторная попытка через 3 секунды!\n");
		printf("[0xGOIDA] Попытка инициализировать номер: %x\n", trying);
		Sleep(3000);
		trying++;
		system("cls");
	}
	TargetProcess.GetBaseAddress("GameAssembly.dll");
	printf("[0xGOIDA] Получил адрес GameAssembly.dll\n");
	TargetProcess.FixCr3();
	printf("[0xGOIDA] Починил CR3\n");


	size_t gameAssembly = TargetProcess.GetBaseAddress("GameAssembly.dll");
	gameAssemblyGlobal = gameAssembly;
	printf("[0xGOIDA] Получил починенный GameAssembly и записал его!\n");

	//OcculusionCulling
	uint64_t OcclusionCullingklass = TargetProcess.Read<uint64_t>(gameAssembly + occlusioncullingOffset);
	uint64_t OcclusionCullingStaticKlass = TargetProcess.Read<uint64_t>(OcclusionCullingklass + 0xb8);
	printf("[0xGOIDA] OcclusionCullingStaticKlass: %llx\n", OcclusionCullingStaticKlass);
	uint64_t OcclusionCullinginstance = TargetProcess.Read<uint64_t>(OcclusionCullingStaticKlass + 0x248);
	uint64_t debugsettings = TargetProcess.Read<uint64_t>(OcclusionCullinginstance + 0x1A8);
	uint64_t debugValue = TargetProcess.Read<uint64_t>(debugsettings + 0x1c);
	printf("[0xGOIDA] DebugValue: %llx\n", debugValue);
	//End

	//Base Player & Player List
	uint64_t basePlayerklass = TargetProcess.Read<uint64_t>(gameAssembly + basePlayerOffset);
	printf("[0xGOIDA] BasePlayerKlass: %llx\n", basePlayerklass);
	uint64_t basePlayerStaticKlass = TargetProcess.Read<uint64_t>(basePlayerklass + 0xb8);
	printf("[0xGOIDA] BasePlayerStaticKlass: %llx\n", basePlayerStaticKlass);
	//End


		
	
	//End
}

