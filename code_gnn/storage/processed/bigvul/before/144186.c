void LockContentsView::LayoutTopHeader() {
  int preferred_width = dev_channel_info_->GetPreferredSize().width() +
                        note_action_->GetPreferredSize().width();
  int preferred_height =
      std::max(dev_channel_info_->GetPreferredSize().height(),
               note_action_->GetPreferredSize().height());
  top_header_->SetPreferredSize(gfx::Size(preferred_width, preferred_height));
  top_header_->SizeToPreferredSize();
  top_header_->Layout();
  top_header_->SetPosition(GetLocalBounds().top_right() -
                           gfx::Vector2d(preferred_width, 0));
}
