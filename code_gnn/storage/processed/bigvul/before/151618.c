void ResourceLoader::Restart(const ResourceRequest& request) {
  CHECK_EQ(resource_->Options().synchronous_policy, kRequestAsynchronously);

  loader_ =
      Context().CreateURLLoader(request, GetTaskRunnerFor(request, Context()));
  StartWith(request);
}
