ChromeContentBrowserClient::GetServiceManifestOverlay(base::StringPiece name) {
  if (name == content::mojom::kBrowserServiceName) {
    return GetChromeContentBrowserOverlayManifest();
  } else if (name == content::mojom::kGpuServiceName) {
    return GetChromeContentGpuOverlayManifest();
  } else if (name == content::mojom::kPackagedServicesServiceName) {
    service_manager::Manifest overlay;
    overlay.packaged_services = GetChromePackagedServiceManifests();
    return overlay;
  } else if (name == content::mojom::kRendererServiceName) {
    return GetChromeContentRendererOverlayManifest();
  } else if (name == content::mojom::kUtilityServiceName) {
    return GetChromeContentUtilityOverlayManifest();
  }

  return base::nullopt;
}
