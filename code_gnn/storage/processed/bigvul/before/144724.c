base::TimeDelta LocalSiteCharacteristicsDataImpl::FeatureObservationDuration(
    const SiteCharacteristicsFeatureProto& feature_proto) const {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  base::TimeDelta observation_time_for_feature =
      InternalRepresentationToTimeDelta(feature_proto.observation_duration());

  if (loaded_tabs_in_background_count_ > 0U &&
      InternalRepresentationToTimeDelta(feature_proto.use_timestamp())
          .is_zero()) {
    base::TimeDelta observation_time_since_backgrounded =
        NowTicks() - background_session_begin_;
    observation_time_for_feature += observation_time_since_backgrounded;
  }

  return observation_time_for_feature;
}
