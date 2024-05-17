void RenderWidgetHostViewAura::OnTextInputStateChanged(
    const ViewHostMsg_TextInputState_Params& params) {
  if (params.show_ime_if_needed && params.type != ui::TEXT_INPUT_TYPE_NONE) {
    if (GetInputMethod())
      GetInputMethod()->ShowImeIfNeeded();
  }
}
