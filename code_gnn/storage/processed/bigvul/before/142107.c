GoogleMdmEnrolledStatusForTesting::~GoogleMdmEnrolledStatusForTesting() {
  g_enrolled_status = EnrolledStatus::kDontForce;
}
