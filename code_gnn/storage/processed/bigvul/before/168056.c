bool AutofillManager::IsFormNonSecure(const FormData& form) const {
  return !client_->IsContextSecure() ||
         (form.action.is_valid() && form.action.SchemeIs("http"));
}
