void NormalizeDisposition(browser::NavigateParams* params) {
  if (params->browser->tabstrip_model()->empty() &&
      (params->disposition == NEW_BACKGROUND_TAB ||
       params->disposition == CURRENT_TAB ||
       params->disposition == SINGLETON_TAB)) {
    params->disposition = NEW_FOREGROUND_TAB;
  }
  if (params->browser->profile()->IsOffTheRecord() &&
      params->disposition == OFF_THE_RECORD) {
    params->disposition = NEW_FOREGROUND_TAB;
  }

  switch (params->disposition) {
    case NEW_BACKGROUND_TAB:
      params->tabstrip_add_types &= ~TabStripModel::ADD_ACTIVE;
      break;

    case NEW_WINDOW:
    case NEW_POPUP:
      if (params->window_action == browser::NavigateParams::NO_ACTION)
        params->window_action = browser::NavigateParams::SHOW_WINDOW;
    case NEW_FOREGROUND_TAB:
    case SINGLETON_TAB:
      params->tabstrip_add_types |= TabStripModel::ADD_ACTIVE;
      break;

    default:
      break;
  }
}
