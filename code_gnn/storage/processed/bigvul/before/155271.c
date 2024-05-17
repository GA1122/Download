bool StartHandlerForClient(int fd) {
  return HandlerStarter::Get()->StartHandlerForClient(GetCrashReporterClient(),
                                                      fd);
}
