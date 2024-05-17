void Compositor::DidSubmitCompositorFrame() {
  base::TimeTicks start_time = base::TimeTicks::Now();
  for (auto& observer : observer_list_)
    observer.OnCompositingStarted(this, start_time);
}
