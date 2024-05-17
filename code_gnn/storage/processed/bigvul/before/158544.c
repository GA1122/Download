WebRect WebLocalFrameImpl::GetSelectionBoundsRectForTesting() const {
  return HasSelection()
             ? WebRect(PixelSnappedIntRect(
                   GetFrame()->Selection().AbsoluteUnclippedBounds()))
             : WebRect();
}
