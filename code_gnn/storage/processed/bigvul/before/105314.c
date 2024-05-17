bool AutofillDownloadManager::StartQueryRequest(
    const ScopedVector<FormStructure>& forms,
    const AutofillMetrics& metric_logger) {
  if (next_query_request_ > base::Time::Now()) {
    return false;
  }
  std::string form_xml;
  FormRequestData request_data;
  if (!FormStructure::EncodeQueryRequest(forms, &request_data.form_signatures,
                                         &form_xml)) {
    return false;
  }

  request_data.request_type = AutofillDownloadManager::REQUEST_QUERY;
  metric_logger.LogServerQueryMetric(AutofillMetrics::QUERY_SENT);

  std::string query_data;
  if (CheckCacheForQueryRequest(request_data.form_signatures, &query_data)) {
    VLOG(1) << "AutofillDownloadManager: query request has been retrieved from"
            << "the cache";
    if (observer_)
      observer_->OnLoadedAutofillHeuristics(query_data);
    return true;
  }

  return StartRequest(form_xml, request_data);
}
