base::string16 BrowserView::GetWindowTitle() const {
  base::string16 title =
      browser_->GetWindowTitleForCurrentTab(true  );
#if defined(OS_MACOSX)
  TabAlertState state =
      chrome::GetTabAlertStateForContents(GetActiveWebContents());
  if (state == TabAlertState::AUDIO_PLAYING)
    title = l10n_util::GetStringFUTF16(IDS_WINDOW_AUDIO_PLAYING_MAC, title,
                                       base::WideToUTF16(L"\U0001F50A"));
  else if (state == TabAlertState::AUDIO_MUTING)
    title = l10n_util::GetStringFUTF16(IDS_WINDOW_AUDIO_MUTING_MAC, title,
                                       base::WideToUTF16(L"\U0001F507"));
#endif
  return title;
}
