void OmniboxViewViews::OnBlur() {
  saved_selection_for_focus_change_ = GetSelectedRange();

  views::Textfield::OnBlur();
  model()->OnWillKillFocus();

  if (!model()->user_input_in_progress() && model()->popup_model() &&
      model()->popup_model()->IsOpen() && text() != model()->PermanentText())
    RevertAll();
  else
    CloseOmniboxPopup();

  model()->OnKillFocus();

  SelectRange(gfx::Range(0));

  gfx::RenderText* render_text = GetRenderText();
  render_text->SetElideBehavior(gfx::ELIDE_TAIL);
  render_text->SetDisplayOffset(0);

  if (location_bar_view_) {
    if (model()->is_keyword_hint())
      location_bar_view_->Layout();

    location_bar_view_->SchedulePaint();
  }
}
