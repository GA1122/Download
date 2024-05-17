 bool Resource::MustRefetchDueToIntegrityMetadata(
     const FetchParameters& params) const {
   if (params.IntegrityMetadata().IsEmpty())
    return false;

  return !IntegrityMetadata::SetsEqual(IntegrityMetadata(),
                                       params.IntegrityMetadata());
}
