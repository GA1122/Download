void InstallablePaymentAppCrawler::OnPaymentWebAppInstallationInfo(
    const GURL& method_manifest_url,
    const GURL& web_app_manifest_url,
    std::unique_ptr<WebAppInstallationInfo> app_info,
    std::unique_ptr<std::vector<PaymentManifestParser::WebAppIcon>> icons) {
  number_of_web_app_manifest_to_parse_--;

  if (CompleteAndStorePaymentWebAppInfoIfValid(
          method_manifest_url, web_app_manifest_url, std::move(app_info))) {
    DownloadAndDecodeWebAppIcon(method_manifest_url, web_app_manifest_url,
                                std::move(icons));
  }

  FinishCrawlingPaymentAppsIfReady();
}
