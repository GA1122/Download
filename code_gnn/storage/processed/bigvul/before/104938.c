void ResourceTracker::InstanceCrashed(PP_Instance instance) {
  CleanupInstanceData(instance, false);
}
