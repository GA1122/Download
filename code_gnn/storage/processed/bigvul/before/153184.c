void Compositor::SetOutputIsSecure(bool output_is_secure) {
  if (context_factory_private_)
    context_factory_private_->SetOutputIsSecure(this, output_is_secure);
}
