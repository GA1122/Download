static int GetCPUCoreCount() {
 int cpuCoreCount = 1;
#if defined(_SC_NPROCESSORS_ONLN)
    cpuCoreCount = sysconf(_SC_NPROCESSORS_ONLN);
#else
    cpuCoreCount = sysconf(_SC_NPROC_ONLN);
#endif
    CHECK_GE(cpuCoreCount, 1);
 return cpuCoreCount;
}
