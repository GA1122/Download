void ExtensionInstallPrompt::Prompt::SetIsShowingDetails(
    DetailsType type,
    size_t index,
    bool is_showing_details) {
  switch (type) {
    case PERMISSIONS_DETAILS:
      prompt_permissions_.is_showing_details[index] = is_showing_details;
      break;
    case WITHHELD_PERMISSIONS_DETAILS:
      withheld_prompt_permissions_.is_showing_details[index] =
          is_showing_details;
      break;
    case RETAINED_FILES_DETAILS:
      is_showing_details_for_retained_files_ = is_showing_details;
      break;
    case RETAINED_DEVICES_DETAILS:
      is_showing_details_for_retained_devices_ = is_showing_details;
      break;
  }
}
