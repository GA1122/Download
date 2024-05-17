CardUnmaskPromptViews::CardUnmaskPromptViews(
    CardUnmaskPromptController* controller)
    : controller_(controller),
      main_contents_(nullptr),
      permanent_error_label_(nullptr),
      input_row_(nullptr),
      cvc_input_(nullptr),
      month_input_(nullptr),
      year_input_(nullptr),
      error_label_(nullptr),
      storage_row_(nullptr),
      storage_checkbox_(nullptr),
      progress_overlay_(nullptr),
      progress_throbber_(nullptr),
      progress_label_(nullptr),
      overlay_animation_(this),
      weak_ptr_factory_(this) {
}
