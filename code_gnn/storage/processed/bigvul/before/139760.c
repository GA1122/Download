void ResourceFetcher::WarnUnusedPreloads() {
  if (!preloads_)
    return;
  for (const auto& resource : *preloads_) {
    if (resource && resource->IsLinkPreload() &&
        resource->GetPreloadResult() == Resource::kPreloadNotReferenced) {
      Context().AddConsoleMessage(
          "The resource " + resource->Url().GetString() +
              " was preloaded using link preload but not used within a few "
              "seconds from the window's load event. Please make sure it "
              "wasn't preloaded for nothing.",
          FetchContext::kLogWarningMessage);
    }
  }
}
