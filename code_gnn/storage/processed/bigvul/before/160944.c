std::unique_ptr<WebFrameScheduler> ChromeClientImpl::CreateFrameScheduler(
    BlameContext* blame_context,
    WebFrameScheduler::FrameType frame_type) {
  return web_view_->Scheduler()->CreateFrameScheduler(blame_context,
                                                      frame_type);
}
