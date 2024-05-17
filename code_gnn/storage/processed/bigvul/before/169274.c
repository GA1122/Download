void ScopedSimulateModifierKeyPress::MouseClickAt(
    int additional_modifiers,
    blink::WebMouseEvent::Button button,
    const gfx::Point& point) {
  SimulateMouseClickAt(web_contents_, modifiers_ | additional_modifiers, button,
                       point);
}
