void ProfilingProcessHost::Unregister() {
  DCHECK(is_registered_);
  Remove(this);
}
