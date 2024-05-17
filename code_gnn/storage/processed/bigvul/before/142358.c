CreateEntry(const std::string& username, const std::string& password) {
  PasswordForm form;
  form.username_value = ASCIIToUTF16(username);
  form.password_value = ASCIIToUTF16(password);
  std::unique_ptr<const PasswordForm> form_ptr(
      new PasswordForm(std::move(form)));
  auto username_form_pair =
      std::make_pair(ASCIIToUTF16(username), form_ptr.get());
  return {std::move(username_form_pair), std::move(form_ptr)};
}
