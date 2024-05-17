void TabStrip::UpdateLoadingAnimations(const base::TimeDelta& elapsed_time) {
  for (int i = 0; i < tab_count(); i++)
    tab_at(i)->StepLoadingAnimation(elapsed_time);
}
