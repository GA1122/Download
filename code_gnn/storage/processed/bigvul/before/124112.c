void ChromeContentBrowserClient::GuestWebContentsCreated(
    WebContents* guest_web_contents, WebContents* embedder_web_contents) {
  Profile* profile = Profile::FromBrowserContext(
      embedder_web_contents->GetBrowserContext());
  ExtensionService* service =
      extensions::ExtensionSystem::Get(profile)->extension_service();
  if (!service)
    return;
  const GURL& url = embedder_web_contents->GetSiteInstance()->GetSiteURL();
  const Extension* extension = service->extensions()->
      GetExtensionOrAppByURL(ExtensionURLInfo(url));
  if (!extension)
    return;
  std::vector<ExtensionMsg_Loaded_Params> extensions;
  extensions.push_back(ExtensionMsg_Loaded_Params(extension));
  guest_web_contents->Send(new ExtensionMsg_Loaded(extensions));
}
