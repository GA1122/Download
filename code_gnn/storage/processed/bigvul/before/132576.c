void WebKitTestController::OnCloseDevTools() {
  if (devtools_frontend_)
    devtools_frontend_->DisconnectFromTarget();
}
