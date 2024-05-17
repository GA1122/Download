void DataUseUserData::AttachToFetcher(net::URLFetcher* fetcher,
                                      ServiceName service_name) {
  fetcher->SetURLRequestUserData(kUserDataKey,
                                 base::Bind(&Create, service_name));
}
