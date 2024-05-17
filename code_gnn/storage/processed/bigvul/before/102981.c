bool TabStripModel::IsNewTabAtEndOfTabStrip(
    TabContentsWrapper* contents) const {
  return LowerCaseEqualsASCII(contents->tab_contents()->GetURL().spec(),
                              chrome::kChromeUINewTabURL) &&
      contents == GetContentsAt(count() - 1) &&
       contents->controller().entry_count() == 1;
 }
