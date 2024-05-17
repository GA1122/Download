bool AreAllTabsMuted(const TabStripModel& tab_strip,
                     const std::vector<int>& indices) {
  for (std::vector<int>::const_iterator i = indices.begin(); i != indices.end();
       ++i) {
    if (!tab_strip.GetWebContentsAt(*i)->IsAudioMuted())
      return false;
  }
  return true;
}
