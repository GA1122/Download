void MetricsLog::WriteMetricsEnableDefault(EnableMetricsDefault metrics_default,
                                           SystemProfileProto* system_profile) {
  if (client_->IsReportingPolicyManaged()) {
    system_profile->set_uma_default_state(
        SystemProfileProto_UmaDefaultState_POLICY_FORCED_ENABLED);
    return;
  }

  switch (metrics_default) {
    case EnableMetricsDefault::DEFAULT_UNKNOWN:
      break;
    case EnableMetricsDefault::OPT_IN:
      system_profile->set_uma_default_state(
          SystemProfileProto_UmaDefaultState_OPT_IN);
      break;
    case EnableMetricsDefault::OPT_OUT:
      system_profile->set_uma_default_state(
          SystemProfileProto_UmaDefaultState_OPT_OUT);
  }
}
