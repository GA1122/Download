void LocalFrameClientImpl::Detached(FrameDetachType type) {
  WebLocalFrameClient* client = web_frame_->Client();
  if (!client)
    return;

  web_frame_->WillDetachParent();

  web_frame_->SetClient(nullptr);

  client->FrameDetached(static_cast<WebLocalFrameClient::DetachType>(type));

  if (type == FrameDetachType::kRemove)
    web_frame_->DetachFromParent();

  web_frame_->SetCoreFrame(nullptr);
}
