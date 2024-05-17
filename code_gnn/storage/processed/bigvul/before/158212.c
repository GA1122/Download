void InstallablePaymentAppCrawler::OnPaymentWebAppManifestDownloaded(
    const GURL& method_manifest_url,
    const GURL& web_app_manifest_url,
    const std::string& content) {
  number_of_web_app_manifest_to_download_--;
  if (content.empty()) {
    FinishCrawlingPaymentAppsIfReady();
    return;
  }

  number_of_web_app_manifest_to_parse_++;
  parser_->ParseWebAppInstallationInfo(
      content,
      base::BindOnce(
          &InstallablePaymentAppCrawler::OnPaymentWebAppInstallationInfo,
          weak_ptr_factory_.GetWeakPtr(), method_manifest_url,
          web_app_manifest_url));
}
