void OmniboxViewViews::SetAccessibilityLabel(const base::string16& display_text,
                                             const AutocompleteMatch& match) {
  bool is_tab_switch_button_focused =
      model()->popup_model()->selected_line_state() ==
      OmniboxPopupModel::BUTTON_FOCUSED;
  friendly_suggestion_text_ = AutocompleteMatchType::ToAccessibilityLabel(
      match, display_text, model()->popup_model()->selected_line(),
      model()->result().size(), is_tab_switch_button_focused,
      &friendly_suggestion_text_prefix_length_);

#if defined(OS_MACOSX)
  GetViewAccessibility().AnnounceText(friendly_suggestion_text_);
#endif
}
