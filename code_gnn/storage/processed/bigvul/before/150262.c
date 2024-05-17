uint8_t GenerateModifierValueForRulebased(bool shift, bool altgr, bool caps) {
  uint8_t modifiers = 0;
  if (shift)
    modifiers |= rulebased::MODIFIER_SHIFT;
  if (altgr)
    modifiers |= rulebased::MODIFIER_ALTGR;
  if (caps)
    modifiers |= rulebased::MODIFIER_CAPSLOCK;
  return modifiers;
}
