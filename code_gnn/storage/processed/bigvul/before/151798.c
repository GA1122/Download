void RecordErrorCauseUMA(const ClientId& client_id, int error_code) {
  base::UmaHistogramSparse(
      AddHistogramSuffix(client_id,
                         "OfflinePages.Background.LoadingErrorStatusCode"),
      error_code);
}
