void CheckClientDownloadRequest::PopulateRequest(
    ClientDownloadRequest* request) {
  request->mutable_digests()->set_sha256(item_->GetHash());
  request->set_length(item_->GetReceivedBytes());
  for (size_t i = 0; i < item_->GetUrlChain().size(); ++i) {
    ClientDownloadRequest::Resource* resource = request->add_resources();
    resource->set_url(SanitizeUrl(item_->GetUrlChain()[i]));
    if (i == item_->GetUrlChain().size() - 1) {
      resource->set_type(ClientDownloadRequest::DOWNLOAD_URL);
      resource->set_referrer(SanitizeUrl(item_->GetReferrerUrl()));
      DVLOG(2) << "dl url " << resource->url();
      if (!item_->GetRemoteAddress().empty()) {
        resource->set_remote_ip(item_->GetRemoteAddress());
        DVLOG(2) << "  dl url remote addr: " << resource->remote_ip();
      }
      DVLOG(2) << "dl referrer " << resource->referrer();
    } else {
      DVLOG(2) << "dl redirect " << i << " " << resource->url();
      resource->set_type(ClientDownloadRequest::DOWNLOAD_REDIRECT);
    }
  }

  request->set_user_initiated(item_->HasUserGesture());

  auto* referrer_chain_data = static_cast<ReferrerChainData*>(
      item_->GetUserData(ReferrerChainData::kDownloadReferrerChainDataKey));
  if (referrer_chain_data &&
      !referrer_chain_data->GetReferrerChain()->empty()) {
    request->mutable_referrer_chain()->Swap(
        referrer_chain_data->GetReferrerChain());
    request->mutable_referrer_chain_options()
        ->set_recent_navigations_to_collect(
            referrer_chain_data->recent_navigations_to_collect());
    UMA_HISTOGRAM_COUNTS_100(
        "SafeBrowsing.ReferrerURLChainSize.DownloadAttribution",
        referrer_chain_data->referrer_chain_length());
  }
}
