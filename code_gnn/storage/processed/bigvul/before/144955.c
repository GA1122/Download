void RenderWidgetHostViewAura::OnSelectionBoundsChanged(
    TextInputManager* text_input_manager,
    RenderWidgetHostViewBase* updated_view) {
  if (GetInputMethod())
    GetInputMethod()->OnCaretBoundsChanged(this);
}
