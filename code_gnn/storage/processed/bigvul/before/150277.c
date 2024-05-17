void PasswordAutofillAgent::CleanupOnDocumentShutdown() {
  web_input_to_password_info_.clear();
  password_to_username_.clear();
  last_supplied_password_info_iter_ = web_input_to_password_info_.end();
  browser_has_form_to_process_ = false;
  field_data_manager_.ClearData();
  username_autofill_state_ = WebAutofillState::kNotFilled;
  password_autofill_state_ = WebAutofillState::kNotFilled;
  sent_request_to_store_ = false;
  checked_safe_browsing_reputation_ = false;
  username_query_prefix_.clear();
  username_detector_cache_.clear();
  forms_structure_cache_.clear();
  autofilled_elements_cache_.clear();
  last_updated_field_renderer_id_ = FormData::kNotSetFormRendererId;
  last_updated_form_renderer_id_ = FormData::kNotSetFormRendererId;
  was_touch_to_fill_ui_shown_ = false;
#if !defined(OS_ANDROID) && !defined(OS_IOS)
  page_passwords_analyser_.Reset();
#endif
}
