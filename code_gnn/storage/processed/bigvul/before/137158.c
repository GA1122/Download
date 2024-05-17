bool Textfield::ChangeTextDirectionAndLayoutAlignment(
    base::i18n::TextDirection direction) {
  const gfx::DirectionalityMode mode = direction == base::i18n::RIGHT_TO_LEFT
                                           ? gfx::DIRECTIONALITY_FORCE_RTL
                                           : gfx::DIRECTIONALITY_FORCE_LTR;
  if (mode == GetRenderText()->directionality_mode())
    GetRenderText()->SetDirectionalityMode(gfx::DIRECTIONALITY_FROM_TEXT);
  else
    GetRenderText()->SetDirectionalityMode(mode);
  SchedulePaint();
  return true;
}
