  explicit AutofillPopupChildView(const Suggestion& suggestion,
                                  int32_t set_size,
                                  int32_t pos_in_set)
      : suggestion_(suggestion),
        is_selected_(false),
        set_size_(set_size),
        pos_in_set_(pos_in_set) {
    SetFocusBehavior(suggestion.frontend_id == POPUP_ITEM_ID_SEPARATOR
                         ? FocusBehavior::NEVER
                         : FocusBehavior::ALWAYS);
  }
