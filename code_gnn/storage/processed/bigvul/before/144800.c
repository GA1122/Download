void TabLifecycleUnitSource::TabLifecycleUnit::RequestFreezeForDiscard(
    DiscardReason reason) {
  DCHECK_EQ(reason, DiscardReason::kProactive);

  SetState(LifecycleUnitState::PENDING_DISCARD,
           DiscardReasonToStateChangeReason(reason));
  EnsureFreezeTimeoutTimerInitialized();
  freeze_timeout_timer_->Start(
      FROM_HERE, kProactiveDiscardFreezeTimeout,
      base::BindRepeating(&TabLifecycleUnit::FinishDiscard,
                          base::Unretained(this), reason));
  GetWebContents()->SetPageFrozen(true);
}
