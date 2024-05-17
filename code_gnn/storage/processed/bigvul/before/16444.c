sysapi_condor_arch(void)
{
	SYSTEM_INFO info;
	GetNativeSystemInfo(&info);
	if (   info.wProcessorArchitecture >= PROCESSOR_ARCHITECTURE_INTEL
		&& info.wProcessorArchitecture <= PROCESSOR_ARCHITECTURE_IA32_ON_WIN64 ) {
		return windows_architectures[info.wProcessorArchitecture];
	} else {
		return unknown_architecture;
	}
}
