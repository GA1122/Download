void Tab::StepLoadingAnimation(const base::TimeDelta& elapsed_time) {
  icon_->StepLoadingAnimation(elapsed_time);

  icon_->SetCanPaintToLayer(controller_->CanPaintThrobberToLayer());
}
