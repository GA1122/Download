 void InputConnectionImpl::DeleteSurroundingText(int before, int after) {
  StartStateUpdateTimer();

  std::string error;
  if (!ime_engine_->DeleteSurroundingText(input_context_id_, -before,
                                          before + after, &error)) {
    LOG(ERROR) << "DeleteSurroundingText failed: before = " << before
               << ", after = " << after << ", error = \"" << error << "\"";
  }
}
