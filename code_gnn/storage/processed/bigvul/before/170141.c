void PasswordAutofillAgent::FrameClosing() {
  for (auto const& iter : web_input_to_password_info_) {
    password_to_username_.erase(iter.second.password_field);
  }
  web_input_to_password_info_.clear();
  provisionally_saved_form_.reset();
  nonscript_modified_values_.clear();
}
