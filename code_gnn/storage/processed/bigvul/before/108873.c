void RenderViewImpl::GetCompositionCharacterBounds(
    std::vector<gfx::Rect>* bounds) {
  DCHECK(bounds);
  bounds->clear();

  if (!webview())
    return;
  size_t start_offset = 0;
  size_t character_count = 0;
  if (!webview()->compositionRange(&start_offset, &character_count))
    return;
  if (character_count == 0)
    return;

  WebKit::WebFrame* frame = webview()->focusedFrame();
  if (!frame)
    return;

  bounds->reserve(character_count);
  WebKit::WebRect webrect;
  for (size_t i = 0; i < character_count; ++i) {
    if (!frame->firstRectForCharacterRange(start_offset + i, 1, webrect)) {
      DLOG(ERROR) << "Could not retrieve character rectangle at " << i;
      bounds->clear();
      return;
    }
    bounds->push_back(webrect);
  }
}
