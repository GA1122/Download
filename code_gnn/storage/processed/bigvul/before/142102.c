GoogleMdmEnrollmentStatusForTesting::GoogleMdmEnrollmentStatusForTesting(
    bool success) {
  g_enrollment_status = success ? EnrollmentStatus::kForceSuccess
                                : EnrollmentStatus::kForceFailure;
}
