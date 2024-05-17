void Compositor::ReenableSwap() {
  DCHECK(context_factory_private_);
  context_factory_private_->ResizeDisplay(this, size_);
}
