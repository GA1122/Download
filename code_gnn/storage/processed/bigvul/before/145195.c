void ChromeExtensionsDispatcherDelegate::RequireAdditionalModules(
    extensions::ScriptContext* context,
    bool is_within_platform_app) {
  extensions::ModuleSystem* module_system = context->module_system();
  extensions::Feature::Context context_type = context->context_type();

  if (context_type == extensions::Feature::BLESSED_EXTENSION_CONTEXT &&
      is_within_platform_app &&
      extensions::GetCurrentChannel() <= version_info::Channel::DEV &&
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          extensions::switches::kEnableAppWindowControls)) {
    module_system->Require("windowControls");
  }

  if (context->GetAvailability("webViewInternal").is_available()) {
    module_system->Require("chromeWebView");
  }
}
