void SessionRestore::RestoreForeignSessionTab(Profile* profile,
    const SessionTab& tab) {
  std::vector<GURL> gurls;
  SessionRestoreImpl restorer(profile,
      static_cast<Browser*>(NULL), true, false, true, gurls);
  restorer.RestoreForeignTab(tab);
}
