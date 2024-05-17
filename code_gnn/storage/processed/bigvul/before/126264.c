void Browser::WebIntentDispatch(
    WebContents* web_contents,
    content::WebIntentsDispatcher* intents_dispatcher) {
  if (!web_intents::IsWebIntentsEnabledForProfile(profile_)) {
    web_intents::RecordIntentsDispatchDisabled();
    delete intents_dispatcher;
    return;
  }

#if !defined(OS_CHROMEOS)
  if (web_contents &&
      !CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kWebIntentsInvocationEnabled)) {
    ExtensionService* extensions_service = profile_->GetExtensionService();
    if (!extensions_service ||
        extensions_service->extensions()->GetExtensionOrAppByURL(
            ExtensionURLInfo(web_contents->GetURL())) == NULL) {
      web_intents::RecordIntentsDispatchDisabled();
      intents_dispatcher->SendReplyMessage(
          webkit_glue::WEB_INTENT_REPLY_FAILURE,
          ASCIIToUTF16("Intents may only be invoked from extensions/apps."));
      return;
    }
  }
#else
  if (intents_dispatcher->GetIntent().action !=
      ASCIIToUTF16(web_intents::kActionCrosEcho) &&
      intents_dispatcher->GetIntent().action !=
      ASCIIToUTF16(web_intents::kActionView)) {
    web_intents::RecordIntentsDispatchDisabled();
    intents_dispatcher->SendReplyMessage(
        webkit_glue::WEB_INTENT_REPLY_FAILURE,
        ASCIIToUTF16("Intents may only be invoked from extensions/apps."));
    return;
  }
#endif

  web_intents::RecordIntentDispatchRequested();

  if (!web_contents) {
    web_contents = chrome::GetActiveWebContents(this);
  }
  WebIntentPickerController* web_intent_picker_controller =
      WebIntentPickerController::FromWebContents(web_contents);
  web_intent_picker_controller->SetIntentsDispatcher(intents_dispatcher);
  web_intent_picker_controller->ShowDialog(
      intents_dispatcher->GetIntent().action,
      intents_dispatcher->GetIntent().type);
}
