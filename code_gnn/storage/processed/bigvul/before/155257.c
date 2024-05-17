bool DumpWithoutCrashingForClient(CrashReporterClient* client) {
  base::ScopedFD connection;
  if (!ConnectToHandler(client, &connection)) {
    return false;
  }

  siginfo_t siginfo;
  siginfo.si_signo = crashpad::Signals::kSimulatedSigno;
  siginfo.si_errno = 0;
  siginfo.si_code = 0;

  ucontext_t context;
  crashpad::CaptureContext(&context);

  crashpad::SanitizationInformation sanitization;
  crashpad::SetSanitizationInfo(client, &sanitization);

  crashpad::ExceptionInformation exception;
  crashpad::SetExceptionInformation(&siginfo, &context, &exception);

  crashpad::ClientInformation info;
  crashpad::SetClientInformation(&exception, &sanitization, &info);

  crashpad::ExceptionHandlerClient handler_client(connection.get());
  return handler_client.RequestCrashDump(info) == 0;
}
