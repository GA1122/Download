SiteFeatureUsage LocalSiteCharacteristicsDataImpl::GetFeatureUsage(
    const SiteCharacteristicsFeatureProto& feature_proto,
    const base::TimeDelta min_obs_time) const {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  if (!feature_proto.IsInitialized())
    return SiteFeatureUsage::kSiteFeatureUsageUnknown;

  if (!InternalRepresentationToTimeDelta(feature_proto.use_timestamp())
           .is_zero()) {
    return SiteFeatureUsage::kSiteFeatureInUse;
  }

  if (FeatureObservationDuration(feature_proto) >= min_obs_time)
    return SiteFeatureUsage::kSiteFeatureNotInUse;

  return SiteFeatureUsage::kSiteFeatureUsageUnknown;
}
