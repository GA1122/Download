void MetricsLog::TruncateEvents() {
  DCHECK(!closed_);
  if (uma_proto_.user_action_event_size() > internal::kUserActionEventLimit) {
    UMA_HISTOGRAM_COUNTS_100000("UMA.TruncatedEvents.UserAction",
                                uma_proto_.user_action_event_size());
    for (int i = internal::kUserActionEventLimit;
         i < uma_proto_.user_action_event_size(); ++i) {
      base::UmaHistogramSparse(
          "UMA.TruncatedEvents.UserAction.Type",
          uma_proto_.user_action_event(i).name_hash() & 0x7fffffff);
    }
    uma_proto_.mutable_user_action_event()->DeleteSubrange(
        internal::kUserActionEventLimit,
        uma_proto_.user_action_event_size() - internal::kUserActionEventLimit);
  }

  if (uma_proto_.omnibox_event_size() > internal::kOmniboxEventLimit) {
    UMA_HISTOGRAM_COUNTS_100000("UMA.TruncatedEvents.Omnibox",
                                uma_proto_.omnibox_event_size());
    uma_proto_.mutable_omnibox_event()->DeleteSubrange(
        internal::kOmniboxEventLimit,
        uma_proto_.omnibox_event_size() - internal::kOmniboxEventLimit);
  }
}
