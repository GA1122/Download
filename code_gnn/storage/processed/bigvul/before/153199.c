bool DesktopWindowTreeHostX11::CaptureSystemKeyEventsImpl(
    base::Optional<base::flat_set<ui::DomCode>> dom_codes) {
  DCHECK(!keyboard_hook_);
  keyboard_hook_ = ui::KeyboardHook::CreateModifierKeyboardHook(
      std::move(dom_codes), GetAcceleratedWidget(),
      base::BindRepeating(&DesktopWindowTreeHostX11::DispatchKeyEvent,
                          base::Unretained(this)));

  return keyboard_hook_ != nullptr;
}
