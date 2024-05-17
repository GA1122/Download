void Browser::WriteCurrentURLToClipboard() {

  TabContents* contents = GetSelectedTabContents();
  if (!contents->ShouldDisplayURL())
    return;

  chrome_browser_net::WriteURLToClipboard(
      contents->GetURL(),
      profile_->GetPrefs()->GetString(prefs::kAcceptLanguages),
      g_browser_process->clipboard());
}
