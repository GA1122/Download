void WebLocalFrameImpl::SetHasHighMediaEngagement(
    bool has_high_media_engagement) {
  GetFrame()->GetDocument()->SetHasHighMediaEngagement(
      has_high_media_engagement);
}
