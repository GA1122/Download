void WebGLRenderingContextBase::OnErrorMessage(const char* message,
                                               int32_t id) {
  if (synthesized_errors_to_console_)
    PrintGLErrorToConsole(message);
  probe::didFireWebGLErrorOrWarning(canvas(), message);
}
