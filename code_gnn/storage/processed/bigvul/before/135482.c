PasswordAutofillManager::~PasswordAutofillManager() {
  if (deletion_callback_)
    std::move(deletion_callback_).Run();
}