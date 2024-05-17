void ResourceDispatcherHostImpl::CancelRequestsForContext(
    ResourceContext* context) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  DCHECK(context);

  CHECK(ContainsKey(active_resource_contexts_, context));

  typedef std::vector<linked_ptr<ResourceLoader> > LoaderList;
  LoaderList loaders_to_cancel;

  for (LoaderMap::iterator i = pending_loaders_.begin();
       i != pending_loaders_.end();) {
    if (i->second->GetRequestInfo()->GetContext() == context) {
      loaders_to_cancel.push_back(i->second);
      pending_loaders_.erase(i++);
    } else {
      ++i;
    }
  }

  for (BlockedLoadersMap::iterator i = blocked_loaders_map_.begin();
       i != blocked_loaders_map_.end();) {
    BlockedLoadersList* loaders = i->second;
    if (loaders->empty()) {
      ++i;
      continue;
    }
    ResourceRequestInfoImpl* info = loaders->front()->GetRequestInfo();
    if (info->GetContext() == context) {
      blocked_loaders_map_.erase(i++);
      for (BlockedLoadersList::const_iterator it = loaders->begin();
           it != loaders->end(); ++it) {
        linked_ptr<ResourceLoader> loader = *it;
        info = loader->GetRequestInfo();
        DCHECK_EQ(context, info->GetContext());
        IncrementOutstandingRequestsMemoryCost(-1 * info->memory_cost(),
                                               info->GetChildID());
        loaders_to_cancel.push_back(loader);
      }
      delete loaders;
    } else {
      ++i;
    }
  }

#ifndef NDEBUG
  for (LoaderList::iterator i = loaders_to_cancel.begin();
       i != loaders_to_cancel.end(); ++i) {
    DCHECK((*i)->GetRequestInfo()->is_download() || (*i)->is_transferring());
  }
#endif

  loaders_to_cancel.clear();

  for (LoaderMap::const_iterator i = pending_loaders_.begin();
       i != pending_loaders_.end(); ++i) {
    CHECK_NE(i->second->GetRequestInfo()->GetContext(), context);
  }

  for (BlockedLoadersMap::const_iterator i = blocked_loaders_map_.begin();
       i != blocked_loaders_map_.end(); ++i) {
    BlockedLoadersList* loaders = i->second;
    if (!loaders->empty()) {
      ResourceRequestInfoImpl* info = loaders->front()->GetRequestInfo();
      CHECK_NE(info->GetContext(), context);
    }
  }
}
