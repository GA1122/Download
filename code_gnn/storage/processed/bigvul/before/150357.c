ClientControlledShellSurface::CreateNonClientFrameView(views::Widget* widget) {
  ash::wm::WindowState* window_state = GetWindowState();
  std::unique_ptr<ash::wm::ClientControlledState::Delegate> delegate =
      GetFactoryForTesting()
          ? GetFactoryForTesting().Run()
          : std::make_unique<ClientControlledStateDelegate>(this);

  auto window_delegate = std::make_unique<ClientControlledWindowStateDelegate>(
      this, delegate.get());
  auto state =
      std::make_unique<ash::wm::ClientControlledState>(std::move(delegate));
  client_controlled_state_ = state.get();
  window_state->SetStateObject(std::move(state));
  window_state->SetDelegate(std::move(window_delegate));
  ash::NonClientFrameViewAsh* frame_view =
      static_cast<ash::NonClientFrameViewAsh*>(
          CreateNonClientFrameViewInternal(widget,  true));
  immersive_fullscreen_controller_ =
      std::make_unique<ash::ImmersiveFullscreenController>();
  frame_view->InitImmersiveFullscreenControllerForView(
      immersive_fullscreen_controller_.get());
  return frame_view;
}
