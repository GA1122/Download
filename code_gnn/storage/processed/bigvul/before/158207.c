void InstallablePaymentAppCrawler::FinishCrawlingPaymentAppsIfReady() {
  if (number_of_payment_method_manifest_to_download_ != 0 ||
      number_of_payment_method_manifest_to_parse_ != 0 ||
      number_of_web_app_manifest_to_download_ != 0 ||
      number_of_web_app_manifest_to_parse_ != 0 ||
      number_of_web_app_icons_to_download_and_decode_ != 0) {
    return;
  }

  std::move(callback_).Run(std::move(installable_apps_));
  std::move(finished_using_resources_).Run();
}
