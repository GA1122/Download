 void ServiceWorkerNetworkProvider::AttachToDocumentState(
    base::SupportsUserData* datasource_userdata,
    std::unique_ptr<ServiceWorkerNetworkProvider> network_provider) {
  datasource_userdata->SetUserData(&kUserDataKey, network_provider.release());
}
