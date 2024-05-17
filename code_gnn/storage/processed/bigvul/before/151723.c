void Browser::RequestAppBannerFromDevTools(content::WebContents* web_contents) {
  banners::AppBannerManagerDesktop::CreateForWebContents(web_contents);
  banners::AppBannerManagerDesktop* manager =
      banners::AppBannerManagerDesktop::FromWebContents(web_contents);
  manager->RequestAppBanner(web_contents->GetLastCommittedURL(), true);
}
