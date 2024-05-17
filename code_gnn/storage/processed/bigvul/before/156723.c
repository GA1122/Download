  explicit TouchSelectionControllerClientTestWrapper(
      ui::TouchSelectionControllerClient* client)
      : expected_event_(ui::SELECTION_HANDLES_SHOWN), client_(client) {}
