  void AddKeyboardControllerBinding(mojo::ScopedMessagePipeHandle handle) {
    ash::Shell::Get()->ash_keyboard_controller()->BindRequest(
        ash::mojom::KeyboardControllerRequest(std::move(handle)));
  }
