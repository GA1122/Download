scoped_refptr<Resource> ResourceTracker::GetResource(PP_Resource res) const {
  DLOG_IF(ERROR, !CheckIdType(res, PP_ID_TYPE_RESOURCE))
      << res << " is not a PP_Resource.";
  ResourceMap::const_iterator result = live_resources_.find(res);
  if (result == live_resources_.end()) {
    return scoped_refptr<Resource>();
  }
  return result->second.first;
}
