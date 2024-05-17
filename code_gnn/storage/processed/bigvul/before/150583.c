void RecordAuthExpiredHistogram(bool auth_expired) {
  UMA_HISTOGRAM_BOOLEAN(kUMAConfigServiceAuthExpired, auth_expired);
}
