void Compositor::OnChildResizing() {
  for (auto& observer : observer_list_)
    observer.OnCompositingChildResizing(this);
}
