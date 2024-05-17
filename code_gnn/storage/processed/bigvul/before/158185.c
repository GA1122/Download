bool IsLoaderInterceptionEnabled() {
  return base::FeatureList::IsEnabled(network::features::kNetworkService) ||
         blink::ServiceWorkerUtils::IsServicificationEnabled() ||
         signed_exchange_utils::IsSignedExchangeHandlingEnabled();
}
