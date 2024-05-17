void CardUnmaskPromptViews::FadeOutView::SetOpacity(double opacity) {
  opacity_ = opacity;
  SchedulePaint();
}
