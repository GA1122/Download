base::string16 Browser::GetWindowTitleFromWebContents(
    bool include_app_name,
    content::WebContents* contents) const {
  base::string16 title;

  if (contents) {
    title = hosted_app_controller_ ? hosted_app_controller_->GetTitle()
                                   : contents->GetTitle();
    FormatTitleForDisplay(&title);
  }

  if (title.empty() && !is_app())
    title = CoreTabHelper::GetDefaultTitle();

#if defined(OS_MACOSX)
  return title;
#endif

  if (title.empty() && is_app() && include_app_name) {
    return base::UTF8ToUTF16(hosted_app_controller_
                                 ? hosted_app_controller_->GetAppShortName()
                                 : app_name());
  }

  return (!is_app() && include_app_name) ?
      l10n_util::GetStringFUTF16(IDS_BROWSER_WINDOW_TITLE_FORMAT, title):
      title;
}
