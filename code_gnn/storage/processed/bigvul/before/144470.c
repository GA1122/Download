void WebContentsImpl::DidChangeLoadProgress() {
  double load_progress = frame_tree_.load_progress();

  base::TimeDelta min_delay =
      base::TimeDelta::FromMilliseconds(kMinimumDelayBetweenLoadingUpdatesMS);
  bool delay_elapsed = loading_last_progress_update_.is_null() ||
      base::TimeTicks::Now() - loading_last_progress_update_ > min_delay;

  if (load_progress == 0.0 || load_progress == 1.0 || delay_elapsed) {
    loading_weak_factory_.InvalidateWeakPtrs();

    SendChangeLoadProgress();

    if (load_progress == 1.0)
      ResetLoadProgressState();
    return;
  }

  if (loading_weak_factory_.HasWeakPtrs())
    return;

  base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
      FROM_HERE, base::Bind(&WebContentsImpl::SendChangeLoadProgress,
                            loading_weak_factory_.GetWeakPtr()),
      min_delay);
}
