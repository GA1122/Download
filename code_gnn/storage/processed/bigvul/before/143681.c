void RenderWidgetHostImpl::OnTextInputStateChanged(
    const ViewHostMsg_TextInputState_Params& params) {
  if (view_)
    view_->TextInputStateChanged(params);
}
