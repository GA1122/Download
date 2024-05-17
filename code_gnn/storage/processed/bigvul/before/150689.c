PageInfo::~PageInfo() {
  SSLCertificateDecisionsDidRevoke user_decision =
      did_revoke_user_ssl_decisions_ ? USER_CERT_DECISIONS_REVOKED
                                     : USER_CERT_DECISIONS_NOT_REVOKED;
  if (show_ssl_decision_revoke_button_) {
    UMA_HISTOGRAM_ENUMERATION("interstitial.ssl.did_user_revoke_decisions2",
                              user_decision,
                              END_OF_SSL_CERTIFICATE_DECISIONS_DID_REVOKE_ENUM);
  }

  base::UmaHistogramCustomTimes(
      security_state::GetSecurityLevelHistogramName(
          kPageInfoTimePrefix, security_level_),
      base::TimeTicks::Now() - start_time_,
      base::TimeDelta::FromMilliseconds(1), base::TimeDelta::FromHours(1), 100);
  if (did_perform_action_) {
    base::UmaHistogramCustomTimes(
        security_state::GetSecurityLevelHistogramName(
            kPageInfoTimeActionPrefix, security_level_),
        base::TimeTicks::Now() - start_time_,
        base::TimeDelta::FromMilliseconds(1), base::TimeDelta::FromHours(1),
        100);
  } else {
    base::UmaHistogramCustomTimes(
        security_state::GetSecurityLevelHistogramName(
            kPageInfoTimeNoActionPrefix, security_level_),
        base::TimeTicks::Now() - start_time_,
        base::TimeDelta::FromMilliseconds(1), base::TimeDelta::FromHours(1),
        100);
  }
}
