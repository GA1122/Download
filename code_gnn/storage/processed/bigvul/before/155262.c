  void HandleCrashNonFatal(int signo, siginfo_t* siginfo, void* context) {
    base::ScopedFD connection;
    if (ConnectToHandler(signo, &connection) == 0) {
      ExceptionInformation exception_information;
      SetExceptionInformation(siginfo, static_cast<ucontext_t*>(context),
                              &exception_information);

      ClientInformation info;
      SetClientInformation(&exception_information, &sanitization_, &info);

      ExceptionHandlerClient handler_client(connection.get());
      handler_client.SetCanSetPtracer(false);
      handler_client.RequestCrashDump(info);
    }
  }
