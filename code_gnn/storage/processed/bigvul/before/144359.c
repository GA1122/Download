ExtensionInstallPrompt::ExtensionInstallPrompt(content::WebContents* contents)
    : profile_(ProfileForWebContents(contents)),
      ui_loop_(base::MessageLoop::current()),
      extension_(NULL),
      bundle_(NULL),
      install_ui_(extensions::CreateExtensionInstallUI(
          ProfileForWebContents(contents))),
      show_params_(new ExtensionInstallPromptShowParams(contents)),
      delegate_(NULL) {
}
