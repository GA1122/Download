void RecordScanningDuration(const base::TimeDelta& duration) {
  UMA_HISTOGRAM_LONG_TIMES("Bluetooth.Web.RequestDevice.ScanningDuration",
                           duration);
}
