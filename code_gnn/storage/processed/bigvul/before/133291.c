void HandleLaunchLastApp() {
  base::RecordAction(UserMetricsAction("Accel_Launch_Last_App"));
  Shelf::ForPrimaryDisplay()->LaunchAppIndexAt(-1);
 }
