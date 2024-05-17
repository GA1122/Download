ScopedSimulateModifierKeyPress::~ScopedSimulateModifierKeyPress() {
  modifiers_ = SimulateModifierKeysUp(web_contents_, control_, shift_, alt_,
                                      command_, modifiers_);
  DCHECK_EQ(0, modifiers_);
}
