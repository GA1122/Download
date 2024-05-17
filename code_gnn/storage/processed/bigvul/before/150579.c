void DataReductionProxyConfigServiceClient::InitializeOnIOThread(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory) {
  DCHECK(url_loader_factory);
#if defined(OS_ANDROID)
  app_status_listener_ =
      base::android::ApplicationStatusListener::New(base::BindRepeating(
          &DataReductionProxyConfigServiceClient::OnApplicationStateChange,
          base::Unretained(this)));
#endif
  url_loader_factory_ = std::move(url_loader_factory);
  network_connection_tracker_->AddNetworkConnectionObserver(this);
}
