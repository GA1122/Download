void Document::WillChangeFrameOwnerProperties(int margin_width,
                                              int margin_height,
                                              ScrollbarMode scrolling_mode,
                                              bool is_display_none) {
  DCHECK(GetFrame() && GetFrame()->Owner());
  FrameOwner* owner = GetFrame()->Owner();

  if (documentElement()) {
    if (is_display_none != owner->IsDisplayNone())
      documentElement()->LazyReattachIfAttached();
  }

  if (margin_width != owner->MarginWidth()) {
    if (auto* body_element = body()) {
      body_element->SetIntegralAttribute(kMarginwidthAttr, margin_width);
    }
  }
  if (margin_height != owner->MarginHeight()) {
    if (auto* body_element = body()) {
      body_element->SetIntegralAttribute(kMarginheightAttr, margin_height);
    }
  }
  if (scrolling_mode != owner->ScrollingMode() && View()) {
    View()->SetNeedsLayout();
  }
}
