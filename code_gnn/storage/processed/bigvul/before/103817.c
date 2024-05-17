void RenderView::OnImeConfirmComposition(const string16& text) {
  if (pepper_delegate_.IsPluginFocused()) {
    for (size_t i = 0; i < text.size(); ++i) {
      WebKit::WebKeyboardEvent char_event;
      char_event.type = WebKit::WebInputEvent::Char;
      char_event.timeStampSeconds = base::Time::Now().ToDoubleT();
      char_event.modifiers = 0;
      char_event.windowsKeyCode = text[i];
      char_event.nativeKeyCode = text[i];
      char_event.text[0] = text[i];
      char_event.unmodifiedText[0] = text[i];
      if (webwidget_)
        webwidget_->handleInputEvent(char_event);
    }
  } else {
    RenderWidget::OnImeConfirmComposition(text);
  }
}
