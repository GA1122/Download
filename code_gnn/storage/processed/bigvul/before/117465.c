const char* EnterpriseStatusToString(
    policy::CloudPolicySubsystem::PolicySubsystemState state) {
  switch (state) {
    case policy::CloudPolicySubsystem::UNENROLLED:
      return "UNENROLLED";
    case policy::CloudPolicySubsystem::BAD_GAIA_TOKEN:
      return "BAD_GAIA_TOKEN";
    case policy::CloudPolicySubsystem::UNMANAGED:
      return "UNMANAGED";
    case policy::CloudPolicySubsystem::NETWORK_ERROR:
      return "NETWORK_ERROR";
    case policy::CloudPolicySubsystem::LOCAL_ERROR:
      return "LOCAL_ERROR";
    case policy::CloudPolicySubsystem::TOKEN_FETCHED:
      return "TOKEN_FETCHED";
    case policy::CloudPolicySubsystem::SUCCESS:
      return "SUCCESS";
    default:
      return "UNKNOWN_STATE";
  }
}
