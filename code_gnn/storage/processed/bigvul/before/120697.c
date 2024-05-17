const WebviewInfo* GetResourcesInfo(
    const Extension& extension) {
  return static_cast<WebviewInfo*>(
      extension.GetManifestData(keys::kWebviewAccessibleResources));
}
