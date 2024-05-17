void Compositor::DisableSwapUntilResize() {
  DCHECK(context_factory_private_);
  context_factory_private_->DisableSwapUntilResize(this);
  disabled_swap_until_resize_ = true;
}
