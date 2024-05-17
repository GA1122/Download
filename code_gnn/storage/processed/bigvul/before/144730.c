void LocalSiteCharacteristicsDataImpl::InitWithDefaultValues(
    bool only_init_uninitialized_fields) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  for (auto* iter : GetAllFeaturesFromProto(&site_characteristics_)) {
    if (!only_init_uninitialized_fields || !iter->IsInitialized())
      InitSiteCharacteristicsFeatureProtoWithDefaultValues(iter);
  }

  if (!only_init_uninitialized_fields ||
      !site_characteristics_.has_last_loaded()) {
    site_characteristics_.set_last_loaded(
        LocalSiteCharacteristicsDataImpl::TimeDeltaToInternalRepresentation(
            base::TimeDelta()));
  }
}
