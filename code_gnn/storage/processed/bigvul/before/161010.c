bool ChromeClientImpl::ShouldReportDetailedMessageForSource(
    LocalFrame& local_frame,
    const String& url) {
  WebLocalFrameImpl* webframe =
      WebLocalFrameImpl::FromFrame(&local_frame.LocalFrameRoot());
  return webframe && webframe->Client() &&
         webframe->Client()->ShouldReportDetailedMessageForSource(url);
}
