void LocalSiteCharacteristicsDataImpl::NotifyFeatureUsage(
    SiteCharacteristicsFeatureProto* feature_proto) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  DCHECK(IsLoaded());
  DCHECK_GT(loaded_tabs_in_background_count_, 0U);

  feature_proto->set_use_timestamp(
      TimeDeltaToInternalRepresentation(GetTickDeltaSinceEpoch()));
  feature_proto->set_observation_duration(
      LocalSiteCharacteristicsDataImpl::TimeDeltaToInternalRepresentation(
          base::TimeDelta()));
}
