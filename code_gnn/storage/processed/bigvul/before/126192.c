string16 Browser::GetWindowTitleForCurrentTab() const {
  WebContents* contents = chrome::GetActiveWebContents(this);
  string16 title;

  if (contents) {
    title = contents->GetTitle();
    FormatTitleForDisplay(&title);
  }
  if (title.empty())
    title = CoreTabHelper::GetDefaultTitle();

#if defined(OS_MACOSX) || defined(USE_ASH)
  return title;
#else
  return is_app() ?
      title :
      l10n_util::GetStringFUTF16(IDS_BROWSER_WINDOW_TITLE_FORMAT, title);
#endif
}
