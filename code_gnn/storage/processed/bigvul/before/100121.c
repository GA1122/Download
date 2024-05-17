void BrowserActionsContainer::OnBrowserActionVisibilityChanged() {
  resize_gripper_->SetVisible(browser_action_views_.size() > 0);

  toolbar_->Layout();
  toolbar_->SchedulePaint();
}
