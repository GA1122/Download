void WorkerFetchContext::DispatchDidFinishLoading(unsigned long identifier,
                                                  double finish_time,
                                                  int64_t encoded_data_length,
                                                  int64_t decoded_body_length) {
  probe::didFinishLoading(global_scope_, identifier, nullptr, finish_time,
                          encoded_data_length, decoded_body_length);
}
