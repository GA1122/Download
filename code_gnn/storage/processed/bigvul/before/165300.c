bool ChromeContentBrowserClient::HandleExternalProtocol(
    const GURL& url,
    content::ResourceRequestInfo::WebContentsGetter web_contents_getter,
    int child_id,
    content::NavigationUIData* navigation_data,
    bool is_main_frame,
    ui::PageTransition page_transition,
    bool has_user_gesture,
    const std::string& method,
    const net::HttpRequestHeaders& headers) {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  ChromeNavigationUIData* chrome_data =
      static_cast<ChromeNavigationUIData*>(navigation_data);
  if ((extensions::WebViewRendererState::GetInstance()->IsGuest(child_id) ||
       (chrome_data &&
        chrome_data->GetExtensionNavigationUIData()->is_web_view())) &&
      !url.SchemeIs(url::kMailToScheme)) {
    return false;
  }
#endif   

#if defined(OS_ANDROID)
  if (is_main_frame)
    return false;
#endif   

  base::PostTaskWithTraits(FROM_HERE, {BrowserThread::UI},
                           base::BindOnce(&LaunchURL, url, web_contents_getter,
                                          page_transition, has_user_gesture));
  return true;
}
