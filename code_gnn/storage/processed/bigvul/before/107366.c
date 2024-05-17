string16 FindPrepopulateText(TabContents* contents) {
  return FindBarState::GetLastPrepopulateText(contents->profile());
}
