TitleWatcher::TitleWatcher(WebContents* web_contents,
                           const base::string16& expected_title)
    : WebContentsObserver(web_contents) {
  expected_titles_.push_back(expected_title);
}
