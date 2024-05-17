static int init_dlls(void)
{
	DLL_LOAD(Cfgmgr32.dll, CM_Get_Parent, TRUE);
	DLL_LOAD(Cfgmgr32.dll, CM_Get_Child, TRUE);
	DLL_LOAD(Cfgmgr32.dll, CM_Get_Sibling, TRUE);
	DLL_LOAD(Cfgmgr32.dll, CM_Get_Device_IDA, TRUE);
	DLL_LOAD_PREFIXED(OLE32.dll, p, CLSIDFromString, TRUE);
	DLL_LOAD_PREFIXED(SetupAPI.dll, p, SetupDiGetClassDevsA, TRUE);
	DLL_LOAD_PREFIXED(SetupAPI.dll, p, SetupDiEnumDeviceInfo, TRUE);
	DLL_LOAD_PREFIXED(SetupAPI.dll, p, SetupDiEnumDeviceInterfaces, TRUE);
	DLL_LOAD_PREFIXED(SetupAPI.dll, p, SetupDiGetDeviceInterfaceDetailA, TRUE);
	DLL_LOAD_PREFIXED(SetupAPI.dll, p, SetupDiDestroyDeviceInfoList, TRUE);
	DLL_LOAD_PREFIXED(SetupAPI.dll, p, SetupDiOpenDevRegKey, TRUE);
	DLL_LOAD_PREFIXED(SetupAPI.dll, p, SetupDiGetDeviceRegistryPropertyA, TRUE);
	DLL_LOAD_PREFIXED(SetupAPI.dll, p, SetupDiOpenDeviceInterfaceRegKey, TRUE);
	DLL_LOAD_PREFIXED(AdvAPI32.dll, p, RegQueryValueExW, TRUE);
	DLL_LOAD_PREFIXED(AdvAPI32.dll, p, RegCloseKey, TRUE);
	return LIBUSB_SUCCESS;
}
