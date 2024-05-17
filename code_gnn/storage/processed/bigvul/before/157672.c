void ImeObserver::OnKeyEvent(
    const std::string& component_id,
    const InputMethodEngineBase::KeyboardEvent& event,
    IMEEngineHandlerInterface::KeyEventDoneCallback key_data) {
  if (extension_id_.empty())
    return;

  if (!ShouldForwardKeyEvent()) {
    std::move(key_data).Run(false);
    return;
  }

  extensions::InputImeEventRouter* event_router =
      extensions::GetInputImeEventRouter(profile_);
  if (!event_router || !event_router->GetActiveEngine(extension_id_))
    return;
  const std::string request_id =
      event_router->GetActiveEngine(extension_id_)
          ->AddRequest(component_id, std::move(key_data));

  input_ime::KeyboardEvent key_data_value;
  key_data_value.type = input_ime::ParseKeyboardEventType(event.type);
  key_data_value.request_id = request_id;
  if (!event.extension_id.empty())
      key_data_value.extension_id.reset(new std::string(event.extension_id));
  key_data_value.key = event.key;
  key_data_value.code = event.code;
  key_data_value.alt_key.reset(new bool(event.alt_key));
  key_data_value.ctrl_key.reset(new bool(event.ctrl_key));
  key_data_value.shift_key.reset(new bool(event.shift_key));
  key_data_value.caps_lock.reset(new bool(event.caps_lock));

  std::unique_ptr<base::ListValue> args(
      input_ime::OnKeyEvent::Create(component_id, key_data_value));

  DispatchEventToExtension(extensions::events::INPUT_IME_ON_KEY_EVENT,
                           input_ime::OnKeyEvent::kEventName, std::move(args));
}
