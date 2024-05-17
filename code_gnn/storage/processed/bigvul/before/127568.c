int DefaultX11ErrorHandler(Display* d, XErrorEvent* e) {
  if (MessageLoop::current()) {
    MessageLoop::current()->PostTask(
         FROM_HERE,
         base::Bind(&LogErrorEventDescription, d, *e));
  } else {
    LOG(ERROR)
        << "X Error detected: "
        << "serial " << e->serial << ", "
        << "error_code " << static_cast<int>(e->error_code) << ", "
        << "request_code " << static_cast<int>(e->request_code) << ", "
        << "minor_code " << static_cast<int>(e->minor_code);
  }
  return 0;
}
