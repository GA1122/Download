std::string Browser::GetDefaultMediaDeviceID(content::WebContents* web_contents,
                                             content::MediaStreamType type) {
  Profile* profile =
      Profile::FromBrowserContext(web_contents->GetBrowserContext());
  return MediaCaptureDevicesDispatcher::GetInstance()
      ->GetDefaultDeviceIDForProfile(profile, type);
}
