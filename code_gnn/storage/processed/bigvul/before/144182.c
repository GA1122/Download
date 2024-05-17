keyboard::KeyboardController* GetKeyboardControllerForWidget(
    const views::Widget* widget) {
  auto* keyboard_controller = keyboard::KeyboardController::Get();
  if (!keyboard_controller->enabled())
    return nullptr;

  aura::Window* keyboard_window = keyboard_controller->GetRootWindow();
  aura::Window* this_window = widget->GetNativeWindow()->GetRootWindow();
  return keyboard_window == this_window ? keyboard_controller : nullptr;
}
