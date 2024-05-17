void SessionRestore::RestoreForeignSessionWindows(
    Profile* profile,
    std::vector<const SessionWindow*>::const_iterator begin,
    std::vector<const SessionWindow*>::const_iterator end) {
  std::vector<GURL> gurls;
  SessionRestoreImpl restorer(profile,
      static_cast<Browser*>(NULL), true, false, true, gurls);
  restorer.RestoreForeignSession(begin, end);
}
