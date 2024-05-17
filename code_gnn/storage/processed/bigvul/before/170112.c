void InputConnectionImpl::FinishComposingText() {
  StartStateUpdateTimer();

  if (composing_text_.empty()) {
    UpdateTextInputState(true);
    return;
  }

  std::string error;
  if (!ime_engine_->CommitText(input_context_id_,
                               base::UTF16ToUTF8(composing_text_).c_str(),
                               &error)) {
    LOG(ERROR) << "FinishComposingText: CommitText() failed, error=\"" << error
               << "\"";
  }
  composing_text_.clear();
}
