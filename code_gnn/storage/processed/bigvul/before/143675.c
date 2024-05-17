void RenderWidgetHostImpl::OnSetTooltipText(
    const base::string16& tooltip_text,
    WebTextDirection text_direction_hint) {
  base::string16 wrapped_tooltip_text = tooltip_text;
  if (!tooltip_text.empty()) {
    if (text_direction_hint == blink::WebTextDirectionLeftToRight) {
      wrapped_tooltip_text =
          base::i18n::GetDisplayStringInLTRDirectionality(wrapped_tooltip_text);
    } else if (text_direction_hint == blink::WebTextDirectionRightToLeft &&
               !base::i18n::IsRTL()) {
      base::i18n::WrapStringWithRTLFormatting(&wrapped_tooltip_text);
    }
  }
  if (GetView())
    view_->SetTooltipText(wrapped_tooltip_text);
}
