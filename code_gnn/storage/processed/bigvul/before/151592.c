void ResourceFetcher::WarnUnusedPreloads() {
  for (const auto& pair : preloads_) {
    Resource* resource = pair.value;
    if (resource && resource->IsLinkPreload() && resource->IsUnusedPreload()) {
      Context().AddWarningConsoleMessage(
          "The resource " + resource->Url().GetString() +
              " was preloaded using link preload but not used within a few "
              "seconds from the window's load event. Please make sure it "
              "wasn't preloaded for nothing.",
          FetchContext::kJSSource);
    }
  }
}
