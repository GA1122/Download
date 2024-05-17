void ResourceDispatcherHostImpl::CancelRequestsForContext(
    ResourceContext* context) {
  DCHECK(io_thread_task_runner_->BelongsToCurrentThread());
  DCHECK(context);

  typedef std::vector<std::unique_ptr<ResourceLoader>> LoaderList;
  LoaderList loaders_to_cancel;

  for (auto i = pending_loaders_.begin(); i != pending_loaders_.end();) {
    ResourceLoader* loader = i->second.get();
    if (loader->GetRequestInfo()->GetContext() == context) {
      loaders_to_cancel.push_back(std::move(i->second));
      IncrementOutstandingRequestsMemory(-1, *loader->GetRequestInfo());
      if (loader->GetRequestInfo()->keepalive()) {
        keepalive_statistics_recorder_.OnLoadFinished(
            loader->GetRequestInfo()->GetChildID());
      }
      pending_loaders_.erase(i++);
    } else {
      ++i;
    }
  }

  for (auto i = blocked_loaders_map_.begin();
       i != blocked_loaders_map_.end();) {
    BlockedLoadersList* loaders = i->second.get();
    if (loaders->empty()) {
      ++i;
      continue;
    }
    ResourceRequestInfoImpl* info = loaders->front()->GetRequestInfo();
    if (info->GetContext() == context) {
      std::unique_ptr<BlockedLoadersList> deleter(std::move(i->second));
      blocked_loaders_map_.erase(i++);
      for (auto& loader : *loaders) {
        info = loader->GetRequestInfo();
        DCHECK_EQ(context, info->GetContext());
        IncrementOutstandingRequestsMemory(-1, *info);
        loaders_to_cancel.push_back(std::move(loader));
      }
    } else {
      ++i;
    }
  }

#ifndef NDEBUG
  for (const auto& loader : loaders_to_cancel) {
    DCHECK(
        loader->GetRequestInfo()->IsDownload() ||
        loader->GetRequestInfo()->is_stream() ||
        (loader->GetRequestInfo()->detachable_handler() &&
         loader->GetRequestInfo()->detachable_handler()->is_detached()) ||
        loader->GetRequestInfo()->requester_info()->IsBrowserSideNavigation() ||
        loader->GetRequestInfo()->GetResourceType() ==
            RESOURCE_TYPE_SERVICE_WORKER);
  }
#endif

  loaders_to_cancel.clear();
}
