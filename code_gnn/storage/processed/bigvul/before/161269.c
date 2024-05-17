void InputHandler::DispatchKeyEvent(
    const std::string& type,
    Maybe<int> modifiers,
    Maybe<double> timestamp,
    Maybe<std::string> text,
    Maybe<std::string> unmodified_text,
    Maybe<std::string> key_identifier,
    Maybe<std::string> code,
    Maybe<std::string> key,
    Maybe<int> windows_virtual_key_code,
    Maybe<int> native_virtual_key_code,
    Maybe<bool> auto_repeat,
    Maybe<bool> is_keypad,
    Maybe<bool> is_system_key,
    Maybe<int> location,
    std::unique_ptr<DispatchKeyEventCallback> callback) {
  blink::WebInputEvent::Type web_event_type;

  if (type == Input::DispatchKeyEvent::TypeEnum::KeyDown) {
    web_event_type = blink::WebInputEvent::kKeyDown;
  } else if (type == Input::DispatchKeyEvent::TypeEnum::KeyUp) {
    web_event_type = blink::WebInputEvent::kKeyUp;
  } else if (type == Input::DispatchKeyEvent::TypeEnum::Char) {
    web_event_type = blink::WebInputEvent::kChar;
  } else if (type == Input::DispatchKeyEvent::TypeEnum::RawKeyDown) {
    web_event_type = blink::WebInputEvent::kRawKeyDown;
  } else {
    callback->sendFailure(Response::InvalidParams(
        base::StringPrintf("Unexpected event type '%s'", type.c_str())));
    return;
  }

  NativeWebKeyboardEvent event(
      web_event_type,
      GetEventModifiers(modifiers.fromMaybe(blink::WebInputEvent::kNoModifiers),
                        auto_repeat.fromMaybe(false),
                        is_keypad.fromMaybe(false), location.fromMaybe(0)),
      GetEventTimeTicks(std::move(timestamp)));

  if (!SetKeyboardEventText(event.text, std::move(text))) {
    callback->sendFailure(Response::InvalidParams("Invalid 'text' parameter"));
    return;
  }
  if (!SetKeyboardEventText(event.unmodified_text,
                            std::move(unmodified_text))) {
    callback->sendFailure(
        Response::InvalidParams("Invalid 'unmodifiedText' parameter"));
    return;
  }

  if (windows_virtual_key_code.isJust())
    event.windows_key_code = windows_virtual_key_code.fromJust();
  if (native_virtual_key_code.isJust())
    event.native_key_code = native_virtual_key_code.fromJust();
  if (is_system_key.isJust())
    event.is_system_key = is_system_key.fromJust();

  if (code.isJust()) {
    event.dom_code = static_cast<int>(
        ui::KeycodeConverter::CodeStringToDomCode(code.fromJust()));
  }

  if (key.isJust()) {
    event.dom_key = static_cast<int>(
        ui::KeycodeConverter::KeyStringToDomKey(key.fromJust()));
  }

  if (!host_ || !host_->GetRenderWidgetHost()) {
    callback->sendFailure(Response::InternalError());
    return;
  }

  if (event.native_key_code)
    event.os_event = NativeInputEventBuilder::CreateEvent(event);
  else
    event.skip_in_browser = true;

  host_->GetRenderWidgetHost()->Focus();
  input_queued_ = false;
  pending_key_callbacks_.push_back(std::move(callback));
  host_->GetRenderWidgetHost()->ForwardKeyboardEvent(event);
  if (!input_queued_) {
    pending_key_callbacks_.back()->sendSuccess();
    pending_key_callbacks_.pop_back();
  }
}
