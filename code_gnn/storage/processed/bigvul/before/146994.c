WebString WebLocalFrameImpl::GetLayerTreeAsTextForTesting(
    bool show_debug_info) const {
  if (!GetFrame())
    return WebString();

  return WebString(GetFrame()->GetLayerTreeAsTextForTesting(
      show_debug_info ? kLayerTreeIncludesDebugInfo : kLayerTreeNormal));
}
