void RenderWidgetHostImpl::ImeFinishComposingText(bool keep_selection) {
  GetWidgetInputHandler()->ImeFinishComposingText(keep_selection);
}
