void WorkerFetchContext::DispatchDidFail(unsigned long identifier,
                                         const ResourceError& error,
                                         int64_t encoded_data_length,
                                         bool is_internal_request) {
  probe::didFailLoading(global_scope_, identifier, nullptr, error);
}
