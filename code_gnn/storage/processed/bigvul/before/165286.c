bool ChromeContentBrowserClient::CanCreateWindow(
    RenderFrameHost* opener,
    const GURL& opener_url,
    const GURL& opener_top_level_frame_url,
    const url::Origin& source_origin,
    content::mojom::WindowContainerType container_type,
    const GURL& target_url,
    const content::Referrer& referrer,
    const std::string& frame_name,
    WindowOpenDisposition disposition,
    const blink::mojom::WindowFeatures& features,
    bool user_gesture,
    bool opener_suppressed,
    bool* no_javascript_access) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(opener);

  content::WebContents* web_contents =
      content::WebContents::FromRenderFrameHost(opener);
  Profile* profile =
      Profile::FromBrowserContext(web_contents->GetBrowserContext());
  DCHECK(profile);
  *no_javascript_access = false;

  if (container_type == content::mojom::WindowContainerType::BACKGROUND) {
#if BUILDFLAG(ENABLE_EXTENSIONS)
    auto* process_map = extensions::ProcessMap::Get(profile);
    auto* registry = extensions::ExtensionRegistry::Get(profile);
    if (!URLHasExtensionBackgroundPermission(process_map, registry, opener_url,
                                             opener->GetProcess()->GetID())) {
      return false;
    }

    const Extension* extension =
        registry->enabled_extensions().GetExtensionOrAppByURL(opener_url);
    if (extension && !extensions::BackgroundInfo::AllowJSAccess(extension))
      *no_javascript_access = true;
#endif

    return true;
  }

#if BUILDFLAG(ENABLE_EXTENSIONS)
  if (extensions::WebViewRendererState::GetInstance()->IsGuest(
          opener->GetProcess()->GetID())) {
    return true;
  }

  if (target_url.SchemeIs(extensions::kExtensionScheme)) {
    auto* registry = extensions::ExtensionRegistry::Get(profile);
    const Extension* extension =
        registry->enabled_extensions().GetExtensionOrAppByURL(target_url);
    if (extension && extension->is_platform_app()) {
      UMA_HISTOGRAM_ENUMERATION(
          "Extensions.AppLoadedInTab",
          ClassifyAppLoadedInTabSource(opener_url, extension),
          APP_LOADED_IN_TAB_SOURCE_MAX);

      return false;
    }
  }
#endif

#if BUILDFLAG(ENABLE_PLUGINS)
  HostContentSettingsMap* content_settings =
      HostContentSettingsMapFactory::GetForProfile(profile);
  if (FlashDownloadInterception::ShouldStopFlashDownloadAction(
          content_settings, opener_top_level_frame_url, target_url,
          user_gesture)) {
    FlashDownloadInterception::InterceptFlashDownloadNavigation(
        web_contents, opener_top_level_frame_url);
    return false;
  }
#endif

  if (auto* prerender_contents =
          prerender::PrerenderContents::FromWebContents(web_contents)) {
    prerender_contents->Destroy(prerender::FINAL_STATUS_CREATE_NEW_WINDOW);
    return false;
  }

  BlockedWindowParams blocked_params(target_url, source_origin, referrer,
                                     frame_name, disposition, features,
                                     user_gesture, opener_suppressed);
  NavigateParams nav_params = blocked_params.CreateNavigateParams(web_contents);
  if (MaybeBlockPopup(web_contents, opener_top_level_frame_url, &nav_params,
                      nullptr  ,
                      blocked_params.features())) {
    return false;
  }

#if defined(OS_ANDROID)
  auto* single_tab_mode_helper =
      SingleTabModeTabHelper::FromWebContents(web_contents);
  if (single_tab_mode_helper &&
      single_tab_mode_helper->block_all_new_windows()) {
    if (TabAndroid* tab_android = TabAndroid::FromWebContents(web_contents)) {
      tab_android->HandlePopupNavigation(&nav_params);
    }
    return false;
  }
#endif

  return true;
}
