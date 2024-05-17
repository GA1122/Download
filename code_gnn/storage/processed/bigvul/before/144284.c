UserSelectionScreen::UserSelectionScreen(const std::string& display_type)
    : BaseScreen(nullptr, OobeScreen::SCREEN_USER_SELECTION),
      display_type_(display_type),
      weak_factory_(this) {}
