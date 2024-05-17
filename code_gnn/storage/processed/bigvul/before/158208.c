InstallablePaymentAppCrawler::InstallablePaymentAppCrawler(
    content::WebContents* web_contents,
    PaymentManifestDownloader* downloader,
    PaymentManifestParser* parser,
    PaymentManifestWebDataService* cache)
    : WebContentsObserver(web_contents),
      downloader_(downloader),
      parser_(parser),
      number_of_payment_method_manifest_to_download_(0),
      number_of_payment_method_manifest_to_parse_(0),
      number_of_web_app_manifest_to_download_(0),
      number_of_web_app_manifest_to_parse_(0),
      number_of_web_app_icons_to_download_and_decode_(0),
      weak_ptr_factory_(this) {}
