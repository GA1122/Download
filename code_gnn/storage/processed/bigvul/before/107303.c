string16 Browser::GetWindowTitleForCurrentTab() const {
  TabContents* contents = GetSelectedTabContents();
  string16 title;

  if (contents) {
    title = contents->GetTitle();
    FormatTitleForDisplay(&title);
  }
  if (title.empty())
    title = TabContents::GetDefaultTitle();

#if defined(OS_MACOSX) || defined(OS_CHROMEOS)
  return title;
#elif defined(OS_WIN) || defined(OS_LINUX)
  int string_id = IDS_BROWSER_WINDOW_TITLE_FORMAT;
  if (type_ & TYPE_APP)
    string_id = IDS_BROWSER_WINDOW_TITLE_FORMAT_NO_LOGO;
  return l10n_util::GetStringFUTF16(string_id, title);
#endif
}
