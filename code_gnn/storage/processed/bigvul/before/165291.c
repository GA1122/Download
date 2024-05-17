content::PreviewsState ChromeContentBrowserClient::DetermineAllowedPreviews(
    content::PreviewsState initial_state,
    content::NavigationHandle* navigation_handle,
    const GURL& current_navigation_url) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);

  if (!navigation_handle->IsInMainFrame() ||
      navigation_handle->IsSameDocument()) {
    return initial_state;
  }

  if (!current_navigation_url.SchemeIsHTTPOrHTTPS()) {
    return content::PREVIEWS_OFF;
  }

  content::WebContents* web_contents = navigation_handle->GetWebContents();
  content::WebContentsDelegate* delegate = web_contents->GetDelegate();

  auto* browser_context = web_contents->GetBrowserContext();

  PreviewsService* previews_service = PreviewsServiceFactory::GetForProfile(
      Profile::FromBrowserContext(browser_context));
  auto* data_reduction_proxy_settings =
      DataReductionProxyChromeSettingsFactory::GetForBrowserContext(
          browser_context);
  if (!previews_service || !previews_service->previews_ui_service() ||
      !data_reduction_proxy_settings) {
    return content::PREVIEWS_OFF;
  }

  PreviewsUITabHelper* ui_tab_helper =
      PreviewsUITabHelper::FromWebContents(web_contents);
  if (!ui_tab_helper)
    return content::PREVIEWS_OFF;

  DCHECK(!browser_context->IsOffTheRecord());

  previews::PreviewsDeciderImpl* previews_decider_impl =
      previews_service->previews_ui_service()->previews_decider_impl();
  DCHECK(previews_decider_impl);

  content::PreviewsState previews_state = content::PREVIEWS_UNSPECIFIED;

  previews::PreviewsUserData* previews_data =
      ui_tab_helper->GetPreviewsUserData(navigation_handle);

  bool is_redirect = false;
  if (previews_data) {
    is_redirect = !previews_data->server_lite_page_info();
  } else {
    previews_data = ui_tab_helper->CreatePreviewsUserDataForNavigationHandle(
        navigation_handle, previews_decider_impl->GeneratePageId());
  }

  DCHECK(previews_data);

  bool is_reload =
      navigation_handle->GetReloadType() != content::ReloadType::NONE;

  content::PreviewsState server_previews_enabled_state =
      content::SERVER_LOFI_ON | content::SERVER_LITE_PAGE_ON;

  if (is_redirect) {
    previews_state |= (previews_data->allowed_previews_state() &
                       server_previews_enabled_state);
  } else {
    if (previews_decider_impl->ShouldAllowPreviewAtNavigationStart(
            previews_data, current_navigation_url, is_reload,
            previews::PreviewsType::LITE_PAGE)) {
      previews_state |= server_previews_enabled_state;
    }
  }

  previews_state |= previews::DetermineAllowedClientPreviewsState(
      previews_data, current_navigation_url, is_reload, is_redirect,
      data_reduction_proxy_settings->IsDataReductionProxyEnabled(),
      previews_decider_impl);

  if (previews_state & content::PREVIEWS_OFF) {
    previews_data->set_allowed_previews_state(content::PREVIEWS_OFF);
    return content::PREVIEWS_OFF;
  }

  if (previews_state & content::PREVIEWS_NO_TRANSFORM) {
    previews_data->set_allowed_previews_state(content::PREVIEWS_NO_TRANSFORM);
    return content::PREVIEWS_NO_TRANSFORM;
  }

  if (previews_state == content::PREVIEWS_UNSPECIFIED) {
    previews_data->set_allowed_previews_state(content::PREVIEWS_OFF);
    return content::PREVIEWS_OFF;
  }

  content::PreviewsState embedder_state = content::PREVIEWS_UNSPECIFIED;
  if (delegate) {
    delegate->AdjustPreviewsStateForNavigation(web_contents, &embedder_state);
  }

  if (embedder_state != content::PREVIEWS_UNSPECIFIED) {
    previews_state = previews_state & embedder_state;
    if (previews_state == content::PREVIEWS_UNSPECIFIED)
      previews_state = content::PREVIEWS_OFF;
  }
  previews_data->set_allowed_previews_state(previews_state);
  return previews_state;
}
