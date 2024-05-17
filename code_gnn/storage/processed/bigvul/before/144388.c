ExtensionInstallPrompt::Prompt::Prompt(PromptType type)
    : type_(type),
      is_showing_details_for_retained_files_(false),
      is_showing_details_for_retained_devices_(false),
      extension_(NULL),
      bundle_(NULL),
      average_rating_(0.0),
      rating_count_(0),
      show_user_count_(false),
      has_webstore_data_(false) {
}
