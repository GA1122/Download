BlameContext* LocalFrameClientImpl::GetFrameBlameContext() {
  if (WebLocalFrameClient* client = web_frame_->Client())
    return client->GetFrameBlameContext();
  return nullptr;
}
