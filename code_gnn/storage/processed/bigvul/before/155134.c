void OmniboxViewViews::OnBlur() {
  saved_selection_for_focus_change_ = GetSelectedRange();

  OmniboxPopupModel* popup_model = model()->popup_model();
  bool popup_closes_on_blur = true;
  if (popup_model)
    popup_closes_on_blur = popup_model->popup_closes_on_blur();

  if (GetWidget() && GetWidget()->IsActive() && popup_closes_on_blur &&
      ((!model()->user_input_in_progress() &&
        text() != model()->GetPermanentDisplayText()) ||
       (model()->user_input_in_progress() &&
        text() == model()->GetPermanentDisplayText()))) {
    RevertAll();
  }

  views::Textfield::OnBlur();
  model()->OnWillKillFocus();

  if (!model()->user_input_in_progress() && popup_model &&
      popup_model->IsOpen() && text() != model()->GetPermanentDisplayText()) {
    RevertAll();
  } else if (popup_closes_on_blur) {
    CloseOmniboxPopup();
  }

  model()->OnKillFocus();

  SelectRange(gfx::Range(0));

  gfx::RenderText* render_text = GetRenderText();
  render_text->SetElideBehavior(gfx::ELIDE_TAIL);
  render_text->SetDisplayOffset(0);

  if (location_bar_view_) {
    if (model()->is_keyword_hint())
      location_bar_view_->Layout();

    location_bar_view_->OnOmniboxBlurred();

    location_bar_view_->SchedulePaint();
  }

  ClearAccessibilityLabel();
}
