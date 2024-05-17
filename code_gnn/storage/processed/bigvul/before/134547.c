void WebContentsViewAura::SetTouchEditableForTest(
    TouchEditableImplAura* touch_editable) {
  touch_editable_.reset(touch_editable);
  AttachTouchEditableToRenderView();
}
