void FaviconWebUIHandler::RegisterMessages() {
  web_ui_->RegisterMessageCallback("getFaviconDominantColor",
      NewCallback(this, &FaviconWebUIHandler::HandleGetFaviconDominantColor));
}
