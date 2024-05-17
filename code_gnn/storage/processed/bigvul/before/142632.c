  static void Reject() {
    g_done_callback->Run(ExtensionInstallPrompt::Result::USER_CANCELED);
  }
