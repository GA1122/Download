void NormalizeDisposition(NavigateParams* params) {
  if (params->browser->tab_strip_model()->empty() &&
      (params->disposition == WindowOpenDisposition::NEW_BACKGROUND_TAB ||
       params->disposition == WindowOpenDisposition::CURRENT_TAB ||
       params->disposition == WindowOpenDisposition::SINGLETON_TAB)) {
    params->disposition = WindowOpenDisposition::NEW_FOREGROUND_TAB;
  }
  if (params->browser->profile()->IsOffTheRecord() &&
      params->disposition == WindowOpenDisposition::OFF_THE_RECORD) {
    params->disposition = WindowOpenDisposition::NEW_FOREGROUND_TAB;
  }
  if (!params->source_contents &&
      params->disposition == WindowOpenDisposition::CURRENT_TAB)
    params->disposition = WindowOpenDisposition::NEW_FOREGROUND_TAB;

  switch (params->disposition) {
    case WindowOpenDisposition::NEW_BACKGROUND_TAB:
      params->tabstrip_add_types &= ~TabStripModel::ADD_ACTIVE;
      break;

    case WindowOpenDisposition::NEW_WINDOW:
    case WindowOpenDisposition::NEW_POPUP: {
      if (params->window_action == NavigateParams::NO_ACTION)
        params->window_action = NavigateParams::SHOW_WINDOW;
      FALLTHROUGH;
    }
    case WindowOpenDisposition::NEW_FOREGROUND_TAB:
    case WindowOpenDisposition::SINGLETON_TAB:
      params->tabstrip_add_types |= TabStripModel::ADD_ACTIVE;
      break;

    default:
      break;
  }
}
