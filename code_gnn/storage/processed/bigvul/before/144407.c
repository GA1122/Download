void ExpandableContainerView::AnimationEnded(const gfx::Animation* animation) {
  if (arrow_toggle_)
    UpdateArrowToggle(animation->GetCurrentValue() != 0.0);
  if (more_details_) {
    more_details_->SetText(expanded_ ?
        l10n_util::GetStringUTF16(IDS_EXTENSIONS_HIDE_DETAILS) :
        l10n_util::GetStringUTF16(IDS_EXTENSIONS_SHOW_DETAILS));
  }
}
