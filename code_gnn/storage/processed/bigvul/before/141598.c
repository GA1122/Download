void EventBindings::OnInvalidated() {
  std::set<std::string> attached_event_names_safe = attached_event_names_;
  for (const std::string& event_name : attached_event_names_safe) {
    DetachEvent(event_name, false  );
  }
  DCHECK(attached_event_names_.empty())
      << "Events cannot be attached during invalidation";

  std::set<int> attached_matcher_ids_safe = attached_matcher_ids_;
  for (int matcher_id : attached_matcher_ids_safe) {
    DetachFilteredEvent(matcher_id, false  );
  }
  DCHECK(attached_matcher_ids_.empty())
      << "Filtered events cannot be attached during invalidation";
}
