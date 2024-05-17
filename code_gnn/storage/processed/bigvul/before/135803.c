SelectionController::SelectionController(LocalFrame& frame)
    : frame_(&frame),
      mouse_down_may_start_select_(false),
      mouse_down_was_single_click_in_selection_(false),
      mouse_down_allows_multi_click_(false),
      selection_state_(SelectionState::kHaveNotStartedSelection) {}
