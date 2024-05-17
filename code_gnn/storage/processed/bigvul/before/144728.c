void LocalSiteCharacteristicsDataImpl::IncrementFeatureObservationDuration(
    SiteCharacteristicsFeatureProto* feature_proto,
    base::TimeDelta extra_observation_duration) {
  if (!feature_proto->has_use_timestamp() ||
      InternalRepresentationToTimeDelta(feature_proto->use_timestamp())
          .is_zero()) {
    feature_proto->set_observation_duration(TimeDeltaToInternalRepresentation(
        InternalRepresentationToTimeDelta(
            feature_proto->observation_duration()) +
        extra_observation_duration));
  }
}
