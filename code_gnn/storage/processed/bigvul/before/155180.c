void OmniboxViewViews::UpdateSchemeStyle(const gfx::Range& range) {
  DCHECK(range.IsValid());
  DCHECK(!model()->user_input_in_progress());

  security_state::SecurityLevel security_level =
      controller()->GetLocationBarModel()->GetSecurityLevel();

  if (security_level == security_state::NONE ||
      security_level == security_state::HTTP_SHOW_WARNING)
    return;
  ApplyColor(location_bar_view_->GetSecurityChipColor(security_level), range);
  if (security_level == security_state::DANGEROUS)
    ApplyStyle(gfx::TEXT_STYLE_STRIKE, true, range);
}
