void InstallablePaymentAppCrawler::OnPaymentMethodManifestDownloaded(
    const GURL& method_manifest_url,
    const std::string& content) {
  number_of_payment_method_manifest_to_download_--;
  if (content.empty()) {
    FinishCrawlingPaymentAppsIfReady();
    return;
  }

  number_of_payment_method_manifest_to_parse_++;
  parser_->ParsePaymentMethodManifest(
      content, base::BindOnce(
                   &InstallablePaymentAppCrawler::OnPaymentMethodManifestParsed,
                   weak_ptr_factory_.GetWeakPtr(), method_manifest_url));
}
