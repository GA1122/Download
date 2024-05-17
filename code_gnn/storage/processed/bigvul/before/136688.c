bool FrameLoader::ShouldPerformFragmentNavigation(bool is_form_submission,
                                                  const String& http_method,
                                                  FrameLoadType load_type,
                                                  const KURL& url) {
  return DeprecatedEqualIgnoringCase(http_method, HTTPNames::GET) &&
         !IsReloadLoadType(load_type) &&
         load_type != kFrameLoadTypeBackForward &&
         url.HasFragmentIdentifier() &&
         EqualIgnoringFragmentIdentifier(frame_->GetDocument()->Url(), url)
         && !frame_->GetDocument()->IsFrameSet();
}
