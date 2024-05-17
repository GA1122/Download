void FetchManager::Loader::PerformSchemeFetch() {
  if (SchemeRegistry::ShouldTreatURLSchemeAsSupportingFetchAPI(
          fetch_request_data_->Url().Protocol()) ||
      fetch_request_data_->Url().ProtocolIs("blob")) {
    PerformHTTPFetch();
  } else if (fetch_request_data_->Url().ProtocolIsData()) {
    PerformDataFetch();
  } else {
    PerformNetworkError(
        "Fetch API cannot load " + fetch_request_data_->Url().GetString() +
        ". URL scheme \"" + fetch_request_data_->Url().Protocol() +
        "\" is not supported.");
  }
}
