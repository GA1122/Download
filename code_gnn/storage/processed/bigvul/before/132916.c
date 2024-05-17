bool RenderWidgetHostViewAura::ChangeTextDirectionAndLayoutAlignment(
      base::i18n::TextDirection direction) {
  if (!host_)
    return false;
  host_->UpdateTextDirection(
      direction == base::i18n::RIGHT_TO_LEFT ?
      blink::WebTextDirectionRightToLeft :
      blink::WebTextDirectionLeftToRight);
  host_->NotifyTextDirection();
  return true;
}
