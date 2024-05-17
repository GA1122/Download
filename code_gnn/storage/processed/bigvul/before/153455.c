void TabStrip::RemoveAndDeleteTab(Tab* tab) {
  std::unique_ptr<Tab> deleter(tab);
  FindClosingTabResult res(FindClosingTab(tab));
  res.first->second.erase(res.second);
  if (res.first->second.empty())
    tabs_closing_map_.erase(res.first);
}
