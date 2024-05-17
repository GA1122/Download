TestLifecycleUnit::TestLifecycleUnit(base::TimeTicks last_focused_time,
                                     base::ProcessHandle process_handle,
                                     bool can_discard)
    : LifecycleUnitBase(content::Visibility::VISIBLE),
      last_focused_time_(last_focused_time),
      process_handle_(process_handle),
      can_discard_(can_discard) {}
