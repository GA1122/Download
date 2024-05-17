void LayerTreeHostImpl::PostDelayedScrollbarAnimationTask(
    const base::Closure& task,
    base::TimeDelta delay) {
  client_->PostDelayedAnimationTaskOnImplThread(task, delay);
}
