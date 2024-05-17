Shell* Shell::CreateInstance(const ShellInitParams& init_params) {
  CHECK(!instance_);
  instance_ = new Shell(init_params.delegate, init_params.blocking_pool);
  instance_->Init(init_params);
  return instance_;
}
