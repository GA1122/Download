TextFinder& WebLocalFrameImpl::EnsureTextFinder() {
  if (!text_finder_)
    text_finder_ = TextFinder::Create(*this);

  return *text_finder_;
}
