void DumpWithoutCrashing() {
  if (g_is_browser) {
    CRASHPAD_SIMULATE_CRASH();
  } else {
    siginfo_t siginfo;
    siginfo.si_signo = crashpad::Signals::kSimulatedSigno;
    siginfo.si_errno = 0;
    siginfo.si_code = 0;

    ucontext_t context;
    crashpad::CaptureContext(&context);

    crashpad::SandboxedHandler::Get()->HandleCrashNonFatal(siginfo.si_signo,
                                                           &siginfo, &context);
  }
}
