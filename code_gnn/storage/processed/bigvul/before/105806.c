void Label::SetHorizontalAlignment(Alignment alignment) {
  if (base::i18n::IsRTL() && (rtl_alignment_mode_ == USE_UI_ALIGNMENT) &&
      (alignment != ALIGN_CENTER))
    alignment = (alignment == ALIGN_LEFT) ? ALIGN_RIGHT : ALIGN_LEFT;
  if (horiz_alignment_ != alignment) {
    horiz_alignment_ = alignment;
    SchedulePaint();
  }
}
