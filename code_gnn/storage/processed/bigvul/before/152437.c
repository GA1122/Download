void RenderFrameImpl::OnImeFinishComposingText(bool keep_selection) {
  const base::string16& text = pepper_composition_text_;
  HandlePepperImeCommit(text);
}
