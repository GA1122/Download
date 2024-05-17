HTMLInputElement::HTMLInputElement(Document& document,
                                   const CreateElementFlags flags)
    : TextControlElement(inputTag, document),
      size_(kDefaultSize),
      has_dirty_value_(false),
      is_checked_(false),
      dirty_checkedness_(false),
      is_indeterminate_(false),
      is_activated_submit_(false),
      autocomplete_(kUninitialized),
      has_non_empty_list_(false),
      state_restored_(false),
      parsing_in_progress_(flags.IsCreatedByParser()),
      can_receive_dropped_files_(false),
      should_reveal_password_(false),
      needs_to_update_view_value_(true),
      is_placeholder_visible_(false),
      has_been_password_field_(false),
      input_type_(flags.IsCreatedByParser() ? nullptr
                                            : InputType::CreateText(*this)),
      input_type_view_(input_type_ ? input_type_->CreateView() : nullptr) {
  SetHasCustomStyleCallbacks();
}
