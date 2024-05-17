bool ExtensionInstallPrompt::Prompt::GetIsShowingDetails(
    DetailsType type, size_t index) const {
  switch (type) {
    case PERMISSIONS_DETAILS:
      CHECK_LT(index, prompt_permissions_.is_showing_details.size());
      return prompt_permissions_.is_showing_details[index];
    case WITHHELD_PERMISSIONS_DETAILS:
      CHECK_LT(index, withheld_prompt_permissions_.is_showing_details.size());
      return withheld_prompt_permissions_.is_showing_details[index];
    case RETAINED_FILES_DETAILS:
      return is_showing_details_for_retained_files_;
    case RETAINED_DEVICES_DETAILS:
      return is_showing_details_for_retained_devices_;
  }
  return false;
}
