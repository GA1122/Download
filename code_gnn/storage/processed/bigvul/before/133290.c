void HandleLaunchAppN(int n) {
  base::RecordAction(UserMetricsAction("Accel_Launch_App"));
  Shelf::ForPrimaryDisplay()->LaunchAppIndexAt(n);
}
