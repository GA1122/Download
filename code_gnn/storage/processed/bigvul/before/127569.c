int DefaultX11IOErrorHandler(Display* d) {
  LOG(ERROR) << "X IO Error detected";
  _exit(1);
}
