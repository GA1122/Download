WebScreenInfo ChromeClientImpl::GetScreenInfo() const {
  return web_view_->Client() ? web_view_->Client()->GetScreenInfo()
                             : WebScreenInfo();
}
