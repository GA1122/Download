WTF::Optional<IntRect> ChromeClientImpl::VisibleContentRectForPainting() const {
  return web_view_->GetDevToolsEmulator()->VisibleContentRectForPainting();
}
