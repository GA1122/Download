void Compositor::BeginMainFrame(const viz::BeginFrameArgs& args) {
  DCHECK(!IsLocked());
  for (auto& observer : animation_observer_list_)
    observer.OnAnimationStep(args.frame_time);
  if (animation_observer_list_.might_have_observers())
    host_->SetNeedsAnimate();
}
