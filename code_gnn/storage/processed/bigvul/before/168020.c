bool FrameLoader::ShouldPerformFragmentNavigation(bool is_form_submission,
                                                  const String& http_method,
                                                  WebFrameLoadType load_type,
                                                  const KURL& url) {
  return DeprecatedEqualIgnoringCase(http_method, HTTPNames::GET) &&
         !IsReloadLoadType(load_type) &&
         load_type != WebFrameLoadType::kBackForward &&
         url.HasFragmentIdentifier() &&
         !frame_->IsProvisional() &&
         EqualIgnoringFragmentIdentifier(frame_->GetDocument()->Url(), url)
         && !frame_->GetDocument()->IsFrameSet();
}
