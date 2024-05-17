WebVector<WebIconURL> WebLocalFrameImpl::IconURLs(int icon_types_mask) const {
  if (GetFrame()->GetDocument()->LoadEventFinished())
    return GetFrame()->GetDocument()->IconURLs(icon_types_mask);
  return WebVector<WebIconURL>();
}
