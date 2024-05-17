void Dispatcher::RequireGuestViewModules(ScriptContext* context) {
  Feature::Context context_type = context->context_type();
  ModuleSystem* module_system = context->module_system();

  if (context->GetAvailability("appViewEmbedderInternal").is_available()) {
    module_system->Require("appView");
  }

  if (context->GetAvailability("extensionOptionsInternal").is_available()) {
    module_system->Require("extensionOptions");
    module_system->Require("extensionOptionsAttributes");
  }

  if (context->GetAvailability("extensionViewInternal").is_available()) {
    module_system->Require("extensionView");
    module_system->Require("extensionViewApiMethods");
    module_system->Require("extensionViewAttributes");
  }

  if (context->GetAvailability("webViewInternal").is_available()) {
    module_system->Require("webView");
    module_system->Require("webViewApiMethods");
    module_system->Require("webViewAttributes");
    if (context->GetAvailability("webViewExperimentalInternal")
            .is_available()) {
      module_system->Require("webViewExperimental");
    }
  }

  if (content::BrowserPluginGuestMode::UseCrossProcessFramesForGuests()) {
    module_system->Require("guestViewIframe");
    module_system->Require("guestViewIframeContainer");
  }

  if (context_type == Feature::BLESSED_EXTENSION_CONTEXT) {
    module_system->Require("guestViewDeny");
  }
}
