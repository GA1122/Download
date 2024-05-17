bool FillDataContainsFillableUsername(const PasswordFormFillData& fill_data) {
  return !fill_data.username_field.name.empty() &&
         (!fill_data.additional_logins.empty() ||
          !fill_data.username_field.value.empty());
}
