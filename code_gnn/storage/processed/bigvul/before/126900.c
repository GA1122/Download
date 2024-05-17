TabStripLayoutType DetermineTabStripLayout(PrefService* prefs,
                                           bool* adjust_layout) {
  *adjust_layout = false;
  if (CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableStackedTabStrip)) {
    return TAB_STRIP_LAYOUT_STACKED;
  }
#if !defined(OS_CHROMEOS)
  if (ui::GetDisplayLayout() != ui::LAYOUT_TOUCH)
    return TAB_STRIP_LAYOUT_SHRINK;
#endif
  *adjust_layout = true;
  switch (prefs->GetInteger(prefs::kTabStripLayoutType)) {
    case TAB_STRIP_LAYOUT_STACKED:
      return TAB_STRIP_LAYOUT_STACKED;
    default:
      return TAB_STRIP_LAYOUT_SHRINK;
  }
}
