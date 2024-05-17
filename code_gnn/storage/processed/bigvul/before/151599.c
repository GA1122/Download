DEFINE_TRACE(ResourceLoader) {
  visitor->Trace(fetcher_);
  visitor->Trace(scheduler_);
  visitor->Trace(resource_);
  ResourceLoadSchedulerClient::Trace(visitor);
}
