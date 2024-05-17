void TitleWatcher::AlsoWaitForTitle(const base::string16& expected_title) {
  expected_titles_.push_back(expected_title);
}
