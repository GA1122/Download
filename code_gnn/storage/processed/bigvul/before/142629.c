  static void Accept() {
    g_done_callback->Run(ExtensionInstallPrompt::Result::ACCEPTED);
  }
