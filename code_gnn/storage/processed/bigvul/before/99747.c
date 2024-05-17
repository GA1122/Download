ResultExpr GpuBrokerProcessPolicy::EvaluateSyscall(int sysno) const {
  switch (sysno) {
#if !defined(__aarch64__)
    case __NR_access:
    case __NR_open:
#endif   
    case __NR_faccessat:
    case __NR_openat:
#if !defined(OS_CHROMEOS)
    case __NR_unlink:
#endif
      return Allow();
    default:
      return GpuProcessPolicy::EvaluateSyscall(sysno);
  }
}
