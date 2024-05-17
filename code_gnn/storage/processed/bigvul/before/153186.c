void Compositor::SetVisible(bool visible) {
  host_->SetVisible(visible);
  if (context_factory_private_)
    context_factory_private_->SetDisplayVisible(this, visible);
}
