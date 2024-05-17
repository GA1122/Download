void BackendImpl::ReportError(int error) {
  STRESS_DCHECK(!error || error == ERR_PREVIOUS_CRASH ||
                error == ERR_CACHE_CREATED);

  DCHECK_LE(error, 0);
  CACHE_UMA(CACHE_ERROR, "Error", 0, error * -1);
}
