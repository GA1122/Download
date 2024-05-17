void RenderFrameImpl::ScrollFocusedEditableElementIntoRect(
    const gfx::Rect& rect) {
  blink::WebAutofillClient* autofill_client = frame_->AutofillClient();
  if (has_scrolled_focused_editable_node_into_rect_ &&
      rect == rect_for_scrolled_focused_editable_node_ && autofill_client) {
    autofill_client->DidCompleteFocusChangeInFrame();
    return;
  }

  if (!render_view_->webview()->ScrollFocusedEditableElementIntoView())
    return;

  rect_for_scrolled_focused_editable_node_ = rect;
  has_scrolled_focused_editable_node_into_rect_ = true;
  if (!GetRenderWidget()->compositor()->HasPendingPageScaleAnimation() &&
      autofill_client) {
    autofill_client->DidCompleteFocusChangeInFrame();
  }
}
