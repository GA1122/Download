StackingController::StackingController() {
  aura::client::SetStackingClient(this);
  always_on_top_controller_.reset(new internal::AlwaysOnTopController);
  always_on_top_controller_->SetContainers(
      GetContainerById(internal::kShellWindowId_DefaultContainer),
      GetContainerById(internal::kShellWindowId_AlwaysOnTopContainer));
}
