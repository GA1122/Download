void LogModelLoadedInTime(bool status) {
  UMA_HISTOGRAM_BOOLEAN("SendTabToSelf.Sync.ModelLoadedInTime", status);
}
