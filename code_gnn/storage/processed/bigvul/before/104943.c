ResourceTracker::ResourceTracker()
    : last_resource_id_(0),
      last_var_id_(0) {
  ::ppapi::TrackerBase::Init(&GetTrackerBase);
}
