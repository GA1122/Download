void InstallablePaymentAppCrawler::DownloadAndDecodeWebAppIcon(
    const GURL& method_manifest_url,
    const GURL& web_app_manifest_url,
    std::unique_ptr<std::vector<PaymentManifestParser::WebAppIcon>> icons) {
  if (icons == nullptr || icons->empty())
    return;

  std::vector<blink::Manifest::ImageResource> manifest_icons;
  for (const auto& icon : *icons) {
    if (icon.src.empty() || !base::IsStringUTF8(icon.src)) {
      WarnIfPossible(
          "The installable payment app's icon src url is not a non-empty UTF8 "
          "string.");
      continue;
    }

    GURL icon_src = GURL(icon.src);
    if (!icon_src.is_valid()) {
      icon_src = web_app_manifest_url.Resolve(icon.src);
      if (!icon_src.is_valid()) {
        WarnIfPossible(
            "Failed to resolve the installable payment app's icon src url (" +
            icon.src + ").");
        continue;
      }
    }

    blink::Manifest::ImageResource manifest_icon;
    manifest_icon.src = icon_src;
    manifest_icon.type = base::UTF8ToUTF16(icon.type);
    manifest_icon.purpose.emplace_back(
        blink::Manifest::ImageResource::Purpose::ANY);
    manifest_icon.sizes.emplace_back(gfx::Size());
    manifest_icons.emplace_back(manifest_icon);
  }

  if (manifest_icons.empty())
    return;

  const int kPaymentAppIdealIconSize = 0xFFFF;
  const int kPaymentAppMinimumIconSize = 0;
  GURL best_icon_url = blink::ManifestIconSelector::FindBestMatchingIcon(
      manifest_icons, kPaymentAppIdealIconSize, kPaymentAppMinimumIconSize,
      blink::Manifest::ImageResource::Purpose::ANY);
  if (!best_icon_url.is_valid()) {
    WarnIfPossible(
        "No suitable icon found in the installabble payment app's manifest (" +
        web_app_manifest_url.spec() + ").");
    return;
  }

  if (web_contents() == nullptr)
    return;

  number_of_web_app_icons_to_download_and_decode_++;
  bool can_download_icon = content::ManifestIconDownloader::Download(
      web_contents(), best_icon_url, kPaymentAppIdealIconSize,
      kPaymentAppMinimumIconSize,
      base::Bind(
          &InstallablePaymentAppCrawler::OnPaymentWebAppIconDownloadAndDecoded,
          weak_ptr_factory_.GetWeakPtr(), method_manifest_url,
          web_app_manifest_url));
  DCHECK(can_download_icon);
}
