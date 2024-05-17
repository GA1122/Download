WebFloatRect WebLocalFrameImpl::ActiveFindMatchRect() {
  if (text_finder_)
    return text_finder_->ActiveFindMatchRect();
  return WebFloatRect();
}
