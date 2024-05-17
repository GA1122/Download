void RenderWidgetHostViewGtk::TextInputStateChanged(
    const ViewHostMsg_TextInputState_Params& params) {
  im_context_->UpdateInputMethodState(params.type, params.can_compose_inline);
}
