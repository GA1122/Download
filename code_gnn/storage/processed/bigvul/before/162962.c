void ManifestManager::BindToRequest(
    blink::mojom::ManifestManagerRequest request) {
  bindings_.AddBinding(this, std::move(request));
}
