void WebLocalFrameImpl::SetHasReceivedUserGesture() {
  if (GetFrame())
    GetFrame()->SetDocumentHasReceivedUserGesture();
}
