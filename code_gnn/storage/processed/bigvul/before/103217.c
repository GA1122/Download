void Browser::UpdateCommandsForContentRestrictionState() {
  int restrictions = GetContentRestrictionsForSelectedTab();

  command_updater_.UpdateCommandEnabled(
      IDC_COPY, !(restrictions & CONTENT_RESTRICTION_COPY));
  command_updater_.UpdateCommandEnabled(
      IDC_CUT, !(restrictions & CONTENT_RESTRICTION_CUT));
  command_updater_.UpdateCommandEnabled(
      IDC_PASTE, !(restrictions & CONTENT_RESTRICTION_PASTE));
  command_updater_.UpdateCommandEnabled(
      IDC_SAVE_PAGE, !(restrictions & CONTENT_RESTRICTION_SAVE));
  UpdatePrintingState(restrictions);
}
