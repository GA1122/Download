    RegisterNonNetworkSubresourceURLLoaderFactories(
        int render_process_id,
        int render_frame_id,
        NonNetworkURLLoaderFactoryMap* factories) {
#if defined(OS_CHROMEOS) || BUILDFLAG(ENABLE_EXTENSIONS)
  content::RenderFrameHost* frame_host =
      RenderFrameHost::FromID(render_process_id, render_frame_id);
  WebContents* web_contents = WebContents::FromRenderFrameHost(frame_host);
#endif   

#if defined(OS_CHROMEOS)
  if (web_contents) {
    Profile* profile =
        Profile::FromBrowserContext(web_contents->GetBrowserContext());
    factories->emplace(content::kExternalFileScheme,
                       std::make_unique<chromeos::ExternalFileURLLoaderFactory>(
                           profile, render_process_id));
  }
#endif   

#if BUILDFLAG(ENABLE_EXTENSIONS)
  auto factory = extensions::CreateExtensionURLLoaderFactory(render_process_id,
                                                             render_frame_id);
  if (factory)
    factories->emplace(extensions::kExtensionScheme, std::move(factory));

  if (!web_contents)
    return;

  Profile* profile =
      Profile::FromBrowserContext(web_contents->GetBrowserContext());
  InstantService* instant_service =
      InstantServiceFactory::GetForProfile(profile);
  if (instant_service->IsInstantProcess(render_process_id)) {
    factories->emplace(
        chrome::kChromeSearchScheme,
        content::CreateWebUIURLLoader(
            frame_host, chrome::kChromeSearchScheme,
             base::flat_set<std::string>()));
  }

  extensions::ChromeExtensionWebContentsObserver* web_observer =
      extensions::ChromeExtensionWebContentsObserver::FromWebContents(
          web_contents);

  if (!web_observer)
    return;

  const Extension* extension =
      web_observer->GetExtensionFromFrame(frame_host, false);
  if (!extension)
    return;

  std::vector<std::string> allowed_webui_hosts;
  if ((extension->is_extension() || extension->is_platform_app()) &&
      Manifest::IsComponentLocation(extension->location())) {
    allowed_webui_hosts.emplace_back(content::kChromeUIResourcesHost);
    allowed_webui_hosts.emplace_back(chrome::kChromeUIThemeHost);
  }
  if (extension->is_extension() || extension->is_legacy_packaged_app() ||
      (extension->is_platform_app() &&
       Manifest::IsComponentLocation(extension->location()))) {
    allowed_webui_hosts.emplace_back(chrome::kChromeUIExtensionIconHost);
    allowed_webui_hosts.emplace_back(chrome::kChromeUIFaviconHost);
  }
  if (!allowed_webui_hosts.empty()) {
    factories->emplace(
        content::kChromeUIScheme,
        content::CreateWebUIURLLoader(frame_host, content::kChromeUIScheme,
                                      std::move(allowed_webui_hosts)));
  }

  extensions::ExtensionHost* host =
      extensions::ProcessManager::Get(web_contents->GetBrowserContext())
          ->GetBackgroundHostForExtension(extension->id());
  if (host) {
    factories->emplace(url::kFileScheme, std::make_unique<FileURLLoaderFactory>(
                                             render_process_id));
  }
#endif   
}
