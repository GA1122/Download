    LocalSiteCharacteristicsWebContentsObserver(
        content::WebContents* web_contents)
    : content::WebContentsObserver(web_contents) {
  if (!g_skip_observer_registration_for_testing) {
    DCHECK(PageSignalReceiver::IsEnabled());

    TabLoadTracker::Get()->AddObserver(this);
    PageSignalReceiver::GetInstance()->AddObserver(this);
  }
}
