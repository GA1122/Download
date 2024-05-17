static void OpenDateTimeChooser(WebView* web_view,
                                HTMLInputElement* input_element) {
  input_element->focus();

  WebKeyboardEvent key_event(WebInputEvent::kRawKeyDown,
                             WebInputEvent::kNoModifiers,
                             WebInputEvent::kTimeStampForTesting);
  key_event.dom_key = Platform::Current()->DomKeyEnumFromString(" ");
  key_event.windows_key_code = VKEY_SPACE;
  web_view->HandleInputEvent(WebCoalescedInputEvent(key_event));

  key_event.SetType(WebInputEvent::kKeyUp);
  web_view->HandleInputEvent(WebCoalescedInputEvent(key_event));
}
