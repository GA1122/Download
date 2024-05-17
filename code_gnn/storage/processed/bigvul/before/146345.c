void WebGLRenderingContextBase::PrintGLErrorToConsole(const String& message) {
  if (!num_gl_errors_to_console_allowed_)
    return;

  --num_gl_errors_to_console_allowed_;
  PrintWarningToConsole(message);

  if (!num_gl_errors_to_console_allowed_)
    PrintWarningToConsole(
        "WebGL: too many errors, no more errors will be reported to the "
        "console for this context.");

  return;
}
