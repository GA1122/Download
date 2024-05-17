    InitSiteCharacteristicsFeatureProtoWithDefaultValues(
        SiteCharacteristicsFeatureProto* proto) {
  DCHECK_NE(nullptr, proto);
  static const auto zero_interval =
      LocalSiteCharacteristicsDataImpl::TimeDeltaToInternalRepresentation(
          base::TimeDelta());
  proto->set_observation_duration(zero_interval);
  proto->set_use_timestamp(zero_interval);
}
