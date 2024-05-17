void ResourceTracker::InstanceDeleted(PP_Instance instance) {
  CleanupInstanceData(instance, true);
}
