void LocalFrameClientImpl::BubbleLogicalScrollInParentFrame(
    ScrollDirection direction,
    ScrollGranularity granularity) {
  web_frame_->Client()->BubbleLogicalScrollInParentFrame(direction,
                                                         granularity);
}
