TabStripMenuController* TabStripGtk::GetTabStripMenuControllerForTab(
    TabGtk* tab) {
  return new TabStripMenuController(tab, model(), GetIndexOfTab(tab));
}
