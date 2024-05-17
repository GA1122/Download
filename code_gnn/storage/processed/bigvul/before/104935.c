::ppapi::ResourceObjectBase* ResourceTracker::GetResourceAPI(
    PP_Resource res) {
  DLOG_IF(ERROR, !CheckIdType(res, PP_ID_TYPE_RESOURCE))
      << res << " is not a PP_Resource.";
  ResourceMap::const_iterator result = live_resources_.find(res);
  if (result == live_resources_.end())
    return NULL;
  return result->second.first.get();
}
