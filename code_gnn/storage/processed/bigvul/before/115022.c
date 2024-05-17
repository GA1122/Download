void TestingAutomationProvider::SendWebKeyPressEventAsync(
    int key_code,
    WebContents* web_contents) {
  NativeWebKeyboardEvent event_down;
  BuildSimpleWebKeyEvent(WebKit::WebInputEvent::RawKeyDown, key_code,
                         &event_down);
  web_contents->GetRenderViewHost()->ForwardKeyboardEvent(event_down);

  NativeWebKeyboardEvent event_up;
  BuildSimpleWebKeyEvent(WebKit::WebInputEvent::KeyUp, key_code, &event_up);
  web_contents->GetRenderViewHost()->ForwardKeyboardEvent(event_up);
}
