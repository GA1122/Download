void NetworkThrottleManagerImpl::OnThrottleDestroyed(ThrottleImpl* throttle) {
  switch (throttle->state()) {
    case ThrottleImpl::State::BLOCKED:
      DCHECK(throttle->queue_pointer() != blocked_throttles_.end());
      DCHECK_EQ(throttle, *(throttle->queue_pointer()));
      blocked_throttles_.erase(throttle->queue_pointer());
      break;
    case ThrottleImpl::State::OUTSTANDING:
      DCHECK(throttle->queue_pointer() != outstanding_throttles_.end());
      DCHECK_EQ(throttle, *(throttle->queue_pointer()));
      outstanding_throttles_.erase(throttle->queue_pointer());
    case ThrottleImpl::State::AGED:
      DCHECK(!throttle->start_time().is_null());
      lifetime_median_estimate_.AddSample(
          (tick_clock_->NowTicks() - throttle->start_time())
              .InMillisecondsRoundedUp());
      break;
  }

  DCHECK(!base::ContainsValue(blocked_throttles_, throttle));
  DCHECK(!base::ContainsValue(outstanding_throttles_, throttle));

  if (outstanding_throttles_.size() < kActiveRequestThrottlingLimit &&
      !blocked_throttles_.empty()) {
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE,
        base::Bind(&NetworkThrottleManagerImpl::MaybeUnblockThrottles,
                   weak_ptr_factory_.GetWeakPtr()));
  }
}
