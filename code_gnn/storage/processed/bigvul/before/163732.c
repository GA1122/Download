std::unique_ptr<WebFrameScheduler> ChromeClientImpl::CreateFrameScheduler(
    BlameContext* blame_context) {
  return web_view_->Scheduler()->CreateFrameScheduler(blame_context);
}
