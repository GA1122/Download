void AutofillManager::Reset() {
  form_structures_.reset();
  has_logged_autofill_enabled_ = false;
  has_logged_address_suggestions_count_ = false;
}
