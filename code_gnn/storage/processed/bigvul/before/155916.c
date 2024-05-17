void ProfileChooserView::WindowClosing() {
  DCHECK_EQ(profile_bubble_, this);
  if (anchor_button_)
    anchor_button_->AnimateInkDrop(views::InkDropState::DEACTIVATED, nullptr);
  profile_bubble_ = NULL;
}
