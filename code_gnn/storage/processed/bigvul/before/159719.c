DevToolsUIBindings::~DevToolsUIBindings() {
  for (const auto& pair : pending_requests_)
    delete pair.first;

  if (agent_host_.get())
    agent_host_->DetachClient(this);

  for (IndexingJobsMap::const_iterator jobs_it(indexing_jobs_.begin());
       jobs_it != indexing_jobs_.end(); ++jobs_it) {
    jobs_it->second->Stop();
  }
  indexing_jobs_.clear();
  SetDevicesUpdatesEnabled(false);

  DevToolsUIBindingsList* instances = g_instances.Pointer();
  DevToolsUIBindingsList::iterator it(
      std::find(instances->begin(), instances->end(), this));
  DCHECK(it != instances->end());
  instances->erase(it);
}
