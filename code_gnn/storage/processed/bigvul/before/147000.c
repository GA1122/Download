WebRect WebLocalFrameImpl::GetSelectionBoundsRectForTesting() const {
  return HasSelection() ? WebRect(IntRect(GetFrame()->Selection().Bounds()))
                        : WebRect();
}
