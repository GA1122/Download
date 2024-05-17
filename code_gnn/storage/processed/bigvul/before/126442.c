void BrowserWindowGtk::ShowWebsiteSettings(
    Profile* profile,
    TabContents* tab_contents,
    const GURL& url,
    const content::SSLStatus& ssl,
    bool show_history) {
    WebsiteSettingsPopupGtk::Show(GetNativeWindow(), profile,
                                  tab_contents, url, ssl);
}
