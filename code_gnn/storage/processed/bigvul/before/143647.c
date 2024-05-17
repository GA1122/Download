void RenderWidgetHostImpl::ImeConfirmComposition(
    const base::string16& text,
    const gfx::Range& replacement_range,
    bool keep_selection) {
  Send(new InputMsg_ImeConfirmComposition(
        GetRoutingID(), text, replacement_range, keep_selection));
}
