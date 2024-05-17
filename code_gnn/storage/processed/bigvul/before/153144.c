void Compositor::DidCommit() {
  DCHECK(!IsLocked());
  for (auto& observer : observer_list_)
    observer.OnCompositingDidCommit(this);
}
