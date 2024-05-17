AutocompleteEditModel::AutocompleteEditModel(
    OmniboxView* view,
    AutocompleteEditController* controller,
    Profile* profile)
    : ALLOW_THIS_IN_INITIALIZER_LIST(
        autocomplete_controller_(new AutocompleteController(profile, this))),
      view_(view),
      popup_(NULL),
      controller_(controller),
      has_focus_(false),
      user_input_in_progress_(false),
      just_deleted_text_(false),
      has_temporary_text_(false),
      paste_state_(NONE),
      control_key_state_(UP),
      is_keyword_hint_(false),
      profile_(profile),
      in_revert_(false),
      allow_exact_keyword_match_(false),
      instant_complete_behavior_(INSTANT_COMPLETE_DELAYED) {
}
