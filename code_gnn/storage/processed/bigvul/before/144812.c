bool IsFocused(LifecycleUnit* lifecycle_unit) {
   return lifecycle_unit->GetLastFocusedTime() == base::TimeTicks::Max();
 }
