void HWNDMessageHandler::Close() {
  if (!IsWindow(hwnd()))
    return;   

  Hide();

  RestoreEnabledIfNecessary();

  if (!waiting_for_close_now_) {
    waiting_for_close_now_ = true;
    base::MessageLoop::current()->PostTask(
        FROM_HERE,
        base::Bind(&HWNDMessageHandler::CloseNow, weak_factory_.GetWeakPtr()));
  }
}
