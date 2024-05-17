void WorkerFetchContext::DispatchDidReceiveData(unsigned long identifier,
                                                const char* data,
                                                int data_length) {
  probe::didReceiveData(global_scope_, identifier, nullptr, data, data_length);
}
