void PasswordAutofillManager::DidNavigateMainFrame() {
  login_to_password_info_.clear();
  did_show_form_not_secure_warning_ = false;
}
