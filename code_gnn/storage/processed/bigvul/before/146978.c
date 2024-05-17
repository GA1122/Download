bool WebLocalFrameImpl::Find(int identifier,
                             const WebString& search_text,
                             const WebFindOptions& options,
                             bool wrap_within_frame,
                             bool* active_now) {
  if (!GetFrame())
    return false;

  DCHECK(GetFrame()->GetPage());

  GetFrame()->GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  return EnsureTextFinder().Find(identifier, search_text, options,
                                 wrap_within_frame, active_now);
}
