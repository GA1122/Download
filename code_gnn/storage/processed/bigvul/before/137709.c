bool PrintViewManagerBase::PrintNowInternal(
    content::RenderFrameHost* rfh,
    std::unique_ptr<IPC::Message> message) {
  if (web_contents()->ShowingInterstitialPage() || web_contents()->IsCrashed())
    return false;
  return rfh->Send(message.release());
}
