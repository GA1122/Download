GoogleMdmEnrollmentStatusForTesting::~GoogleMdmEnrollmentStatusForTesting() {
  g_enrollment_status = EnrollmentStatus::kDontForce;
}
