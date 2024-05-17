OmniboxViewViews::OmniboxViewViews(OmniboxEditController* controller,
                                   Profile* profile,
                                   CommandUpdater* command_updater,
                                   bool popup_window_mode,
                                   LocationBarView* location_bar,
                                   const gfx::FontList& font_list)
    : OmniboxView(profile, controller, command_updater),
      popup_window_mode_(popup_window_mode),
      security_level_(ToolbarModel::NONE),
      saved_selection_for_focus_change_(gfx::Range::InvalidRange()),
      ime_composing_before_change_(false),
      delete_at_end_pressed_(false),
      location_bar_view_(location_bar),
      ime_candidate_window_open_(false),
      select_all_on_mouse_release_(false),
      select_all_on_gesture_tap_(false) {
  set_border(NULL);
  set_id(VIEW_ID_OMNIBOX);
  SetFontList(font_list);
}
