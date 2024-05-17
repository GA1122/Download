void SetupDocumentLoader(
    DocumentLoader* document_loader,
    std::unique_ptr<WebNavigationParams> navigation_params) {
  if (!navigation_params) {
    document_loader->GetTiming().SetNavigationStart(CurrentTimeTicks());
    return;
  }

  const WebNavigationTimings& navigation_timings =
      navigation_params->navigation_timings;
  document_loader->UpdateNavigationTimings(
      navigation_timings.navigation_start, navigation_timings.redirect_start,
      navigation_timings.redirect_end, navigation_timings.fetch_start,
      navigation_timings.input_start);

  document_loader->SetSourceLocation(navigation_params->source_location);
  if (navigation_params->is_user_activated)
    document_loader->SetUserActivated();

  document_loader->SetServiceWorkerNetworkProvider(
      std::move(navigation_params->service_worker_network_provider));
}
