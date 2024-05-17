base::TimeDelta WarmupURLFetcher::GetFetchWaitTime() const {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  DCHECK_LT(0u, previous_attempt_counts_);
  DCHECK_GE(2u, previous_attempt_counts_);

  if (previous_attempt_counts_ == 1)
    return base::TimeDelta::FromSeconds(1);

  return base::TimeDelta::FromSeconds(30);
}
