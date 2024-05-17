ResultExpr GpuProcessPolicy::EvaluateSyscall(int sysno) const {
  switch (sysno) {
#if !defined(OS_CHROMEOS)
    case __NR_ftruncate:
#endif
    case __NR_ioctl:
      return Allow();
    case __NR_mincore:
      if (allow_mincore_) {
        return Allow();
      } else {
        return SandboxBPFBasePolicy::EvaluateSyscall(sysno);
      }
#if defined(__i386__) || defined(__x86_64__) || defined(__mips__)
    case __NR_mmap:
#endif
    case __NR_mprotect:
    case __NR_prctl:
      return Allow();
#if !defined(__aarch64__)
    case __NR_access:
    case __NR_open:
#endif   
    case __NR_faccessat:
    case __NR_openat:
      DCHECK(broker_process_);
      return Trap(GpuSIGSYS_Handler, broker_process_);
    case __NR_sched_getaffinity:
    case __NR_sched_setaffinity:
      return sandbox::RestrictSchedTarget(GetPolicyPid(), sysno);
    default:
      if (SyscallSets::IsEventFd(sysno))
        return Allow();

      return SandboxBPFBasePolicy::EvaluateSyscall(sysno);
  }
}
