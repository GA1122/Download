void InstallablePaymentAppCrawler::OnPaymentWebAppIconDownloadAndDecoded(
    const GURL& method_manifest_url,
    const GURL& web_app_manifest_url,
    const SkBitmap& icon) {
  number_of_web_app_icons_to_download_and_decode_--;
  if (icon.drawsNothing()) {
    WarnIfPossible(
        "Failed to download or decode installable payment app's icon for web "
        "app manifest " +
        web_app_manifest_url.spec() + ".");
  } else {
    std::map<GURL, std::unique_ptr<WebAppInstallationInfo>>::iterator it =
        installable_apps_.find(method_manifest_url);
    DCHECK(it != installable_apps_.end());
    DCHECK(url::IsSameOriginWith(GURL(it->second->sw_scope),
                                 web_app_manifest_url));

    it->second->icon = std::make_unique<SkBitmap>(icon);
  }

  FinishCrawlingPaymentAppsIfReady();
}
