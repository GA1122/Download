    ClearObservationsAndInvalidateReadOperation() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  weak_factory_.InvalidateWeakPtrs();

  InitWithDefaultValues(false);

  if (IsLoaded()) {
    site_characteristics_.set_last_loaded(
        TimeDeltaToInternalRepresentation(GetTickDeltaSinceEpoch()));
  }

  safe_to_write_to_db_ = true;
}
