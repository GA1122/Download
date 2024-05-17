WebLocalFrameImpl* WebLocalFrameImpl::FromFrame(LocalFrame& frame) {
  LocalFrameClient* client = frame.Client();
  if (!client || !client->IsLocalFrameClientImpl())
    return nullptr;
  return ToWebLocalFrameImpl(client->GetWebFrame());
}
