void ChromeContentBrowserClient::ClearInspectorSettings(RenderViewHost* rvh) {
  content::BrowserContext* browser_context =
      rvh->GetProcess()->GetBrowserContext();
  Profile::FromBrowserContext(browser_context)->GetPrefs()->
      ClearPref(prefs::kWebKitInspectorSettings);
}
