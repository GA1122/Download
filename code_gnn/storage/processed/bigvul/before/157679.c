ExtensionFunction::ResponseAction InputImeSendKeyEventsFunction::Run() {
  InputImeEventRouter* event_router =
      GetInputImeEventRouter(Profile::FromBrowserContext(browser_context()));
  InputMethodEngineBase* engine =
      event_router ? event_router->GetActiveEngine(extension_id()) : nullptr;
  if (!engine)
    return RespondNow(Error(kInputImeApiErrorEngineNotAvailable));

  std::unique_ptr<SendKeyEvents::Params> parent_params(
      SendKeyEvents::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(parent_params);
  const SendKeyEvents::Params::Parameters& params = parent_params->parameters;
  std::vector<InputMethodEngineBase::KeyboardEvent> key_data_out;

  for (const auto& key_event : params.key_data) {
    key_data_out.push_back(InputMethodEngineBase::KeyboardEvent());
    InputMethodEngineBase::KeyboardEvent& event = key_data_out.back();
    event.type = input_ime::ToString(key_event.type);
    event.key = key_event.key;
    event.code = key_event.code;
    event.key_code = key_event.key_code.get() ? *(key_event.key_code) : 0;
    event.alt_key = key_event.alt_key ? *(key_event.alt_key) : false;
    event.ctrl_key = key_event.ctrl_key ? *(key_event.ctrl_key) : false;
    event.shift_key = key_event.shift_key ? *(key_event.shift_key) : false;
    event.caps_lock = key_event.caps_lock ? *(key_event.caps_lock) : false;
  }
  if (!engine->SendKeyEvents(params.context_id, key_data_out))
    return RespondNow(Error(kInputImeApiErrorSetKeyEventsFail));
  return RespondNow(NoArguments());
}
