DatabaseMessageFilter::DatabaseMessageFilter(
    webkit_database::DatabaseTracker* db_tracker)
    : db_tracker_(db_tracker),
      observer_added_(false) {
  DCHECK(db_tracker_);
}
