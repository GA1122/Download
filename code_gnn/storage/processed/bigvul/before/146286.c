void WebGLRenderingContextBase::EmitGLWarning(const char* function_name,
                                              const char* description) {
  if (synthesized_errors_to_console_) {
    String message =
        String("WebGL: ") + String(function_name) + ": " + String(description);
    PrintGLErrorToConsole(message);
  }
  probe::didFireWebGLWarning(canvas());
}
