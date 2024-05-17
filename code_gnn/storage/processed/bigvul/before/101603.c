void Browser::ContentRestrictionsChanged(TabContents* source) {
  UpdateCommandsForContentRestrictionState();
}
