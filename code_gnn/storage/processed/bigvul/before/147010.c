void WebLocalFrameImpl::IncreaseMatchCount(int count, int identifier) {
  EnsureTextFinder().IncreaseMatchCount(identifier, count);
}
