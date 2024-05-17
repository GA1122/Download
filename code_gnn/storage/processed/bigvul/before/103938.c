void KeyboardOverlayHandler::RegisterMessages() {
  DCHECK(web_ui_);
  web_ui_->RegisterMessageCallback("getInputMethodId",
      NewCallback(this, &KeyboardOverlayHandler::GetInputMethodId));
  web_ui_->RegisterMessageCallback("getLabelMap",
      NewCallback(this, &KeyboardOverlayHandler::GetLabelMap));
}
