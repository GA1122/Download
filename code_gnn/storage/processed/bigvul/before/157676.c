ExtensionFunction::ResponseAction InputImeKeyEventHandledFunction::Run() {
  std::unique_ptr<KeyEventHandled::Params> params(
      KeyEventHandled::Params::Create(*args_));
  InputImeEventRouter* event_router =
      GetInputImeEventRouter(Profile::FromBrowserContext(browser_context()));
  InputMethodEngineBase* engine =
      event_router ? event_router->GetActiveEngine(extension_id()) : nullptr;
  if (engine) {
    engine->KeyEventHandled(extension_id(), params->request_id,
                            params->response);
  }
  return RespondNow(NoArguments());
}
