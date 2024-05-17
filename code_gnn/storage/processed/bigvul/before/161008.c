void ChromeClientImpl::SetWindowRect(const IntRect& r, LocalFrame& frame) {
  DCHECK_EQ(&frame, web_view_->MainFrameImpl()->GetFrame());
  WebWidgetClient* client =
      WebLocalFrameImpl::FromFrame(&frame)->FrameWidget()->Client();
  client->SetWindowRect(r);
}
