bool FormContainsNonDefaultPasswordValue(const PasswordForm& password_form) {
  return (!password_form.password_value.empty() &&
          !password_form.password_value_is_default) ||
      (!password_form.new_password_value.empty() &&
       !password_form.new_password_value_is_default);
}
