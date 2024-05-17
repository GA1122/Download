void OobeUI::InitializeHandlers() {
  ready_ = true;
  for (size_t i = 0; i < ready_callbacks_.size(); ++i)
    ready_callbacks_[i].Run();
  ready_callbacks_.clear();

  for (size_t i = 0; i < handlers_.size(); ++i) {
    if (handlers_[i]->async_assets_load_id().empty())
      handlers_[i]->InitializeBase();
  }

  shutdown_policy_handler_.reset(
      new ShutdownPolicyHandler(CrosSettings::Get(), this));

  shutdown_policy_handler_->CheckIfRebootOnShutdown(
      base::Bind(&OobeUI::OnShutdownPolicyChanged, base::Unretained(this)));
}
