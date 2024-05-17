  void DidScroll(const gfx::ScrollOffset& offset, const CompositorElementId&) {
    did_scroll_count++;
    last_scroll_offset = offset;
  };
