void ManifestManager::RequestManifest(RequestManifestCallback callback) {
  RequestManifestImpl(base::BindOnce(
      [](RequestManifestCallback callback, const GURL& manifest_url,
         const Manifest& manifest,
         const blink::mojom::ManifestDebugInfo* debug_info) {
        std::move(callback).Run(manifest_url, manifest);
      },
      std::move(callback)));
}
