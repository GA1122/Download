InputMethodBase::InputMethodBase()
  : delegate_(NULL),
    text_input_client_(NULL),
    is_sticky_text_input_client_(false),
    system_toplevel_window_focused_(false) {
}
