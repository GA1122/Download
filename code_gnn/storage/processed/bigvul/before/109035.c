void RenderViewImpl::SimulateImeConfirmComposition(
    const string16& text,
    const ui::Range& replacement_range) {
  OnImeConfirmComposition(text, replacement_range);
}
