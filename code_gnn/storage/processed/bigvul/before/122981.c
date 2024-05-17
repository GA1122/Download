void RenderWidgetHostImpl::ImeConfirmComposition(
    const string16& text, const ui::Range& replacement_range) {
  Send(new ViewMsg_ImeConfirmComposition(
        GetRoutingID(), text, replacement_range));
}
