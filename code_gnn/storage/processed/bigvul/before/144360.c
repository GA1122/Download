ExtensionInstallPrompt::ExtensionInstallPrompt(Profile* profile,
                                               gfx::NativeWindow native_window)
    : profile_(profile),
      ui_loop_(base::MessageLoop::current()),
      extension_(NULL),
      bundle_(NULL),
      install_ui_(extensions::CreateExtensionInstallUI(profile)),
      show_params_(
          new ExtensionInstallPromptShowParams(profile, native_window)),
      delegate_(NULL) {
}
