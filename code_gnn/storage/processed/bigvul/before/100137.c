 int BrowserActionsContainer::WidthOfNonIconArea() const {
   int chevron_size = (chevron_->IsVisible()) ?
                      chevron_->GetPreferredSize().width() : 0;
  return resize_gripper_->GetPreferredSize().width() + kHorizontalPadding +
         chevron_size + kChevronRightMargin + kDividerHorizontalMargin;
}
