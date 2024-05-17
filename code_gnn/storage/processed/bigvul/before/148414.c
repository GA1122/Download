TextInputManager* WebContentsImpl::GetTextInputManager() {
  if (GetOuterWebContents())
    return GetOuterWebContents()->GetTextInputManager();

  if (!text_input_manager_)
    text_input_manager_.reset(new TextInputManager());

  return text_input_manager_.get();
}
