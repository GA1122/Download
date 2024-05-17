bool Document::ShouldScheduleLayoutTreeUpdate() const {
  if (!IsActive())
    return false;
  if (InStyleRecalc())
    return false;
  if (lifecycle_.GetState() == DocumentLifecycle::kInPreLayout)
    return false;
  if (!ShouldScheduleLayout())
    return false;
  return true;
}
