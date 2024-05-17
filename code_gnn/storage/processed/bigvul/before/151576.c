void ResourceFetcher::RemoveResourceLoader(ResourceLoader* loader) {
  DCHECK(loader);
  if (loaders_.Contains(loader))
    loaders_.erase(loader);
  else if (non_blocking_loaders_.Contains(loader))
    non_blocking_loaders_.erase(loader);
  else
    NOTREACHED();

  if (loaders_.IsEmpty() && non_blocking_loaders_.IsEmpty())
    keepalive_loaders_task_handle_.Cancel();
}
