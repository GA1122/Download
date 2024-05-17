void ManifestManager::RequestManifestDebugInfo(
    RequestManifestDebugInfoCallback callback) {
  RequestManifestImpl(base::BindOnce(
      [](RequestManifestDebugInfoCallback callback, const GURL& manifest_url,
         const Manifest& manifest,
         const blink::mojom::ManifestDebugInfo* debug_info) {
        std::move(callback).Run(manifest_url,
                                debug_info ? debug_info->Clone() : nullptr);
      },
      std::move(callback)));
}
