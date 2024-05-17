int WebLocalFrameImpl::FindMatchMarkersVersion() const {
  if (text_finder_)
    return text_finder_->FindMatchMarkersVersion();
  return 0;
}
