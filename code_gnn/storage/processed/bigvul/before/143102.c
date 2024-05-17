ServiceWorkerNetworkProvider* ServiceWorkerNetworkProvider::FromDocumentState(
    base::SupportsUserData* datasource_userdata) {
  return static_cast<ServiceWorkerNetworkProvider*>(
      datasource_userdata->GetUserData(&kUserDataKey));
}
