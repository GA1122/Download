GoogleMdmEnrolledStatusForTesting::GoogleMdmEnrolledStatusForTesting(
    bool success) {
  g_enrolled_status =
      success ? EnrolledStatus::kForceTrue : EnrolledStatus::kForceFalse;
}
