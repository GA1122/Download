void Label::SetElideInMiddle(bool elide_in_middle) {
  DCHECK(!elide_in_middle || !is_multi_line_);
  if (elide_in_middle != elide_in_middle_) {
    elide_in_middle_ = elide_in_middle;
    text_size_valid_ = false;
    PreferredSizeChanged();
    SchedulePaint();
  }
}
