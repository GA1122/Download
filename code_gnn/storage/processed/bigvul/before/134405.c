void TabStrip::RemoveAndDeleteTab(Tab* tab) {
  scoped_ptr<Tab> deleter(tab);
  for (TabsClosingMap::iterator i(tabs_closing_map_.begin());
       i != tabs_closing_map_.end(); ++i) {
    std::vector<Tab*>::iterator j =
        std::find(i->second.begin(), i->second.end(), tab);
    if (j != i->second.end()) {
      i->second.erase(j);
      if (i->second.empty())
        tabs_closing_map_.erase(i);
      return;
    }
  }
  NOTREACHED();
}
