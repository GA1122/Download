void ResourceLoader::DidFinishLoading(double finish_time,
                                      int64_t encoded_data_length,
                                      int64_t encoded_body_length,
                                      int64_t decoded_body_length) {
  resource_->SetEncodedDataLength(encoded_data_length);
  resource_->SetEncodedBodyLength(encoded_body_length);
  resource_->SetDecodedBodyLength(decoded_body_length);

  Release(ResourceLoadScheduler::ReleaseOption::kReleaseAndSchedule);
  loader_.reset();

  network_instrumentation::EndResourceLoad(
      resource_->Identifier(),
      network_instrumentation::RequestOutcome::kSuccess);

  fetcher_->HandleLoaderFinish(resource_.Get(), finish_time,
                               ResourceFetcher::kDidFinishLoading);
}
