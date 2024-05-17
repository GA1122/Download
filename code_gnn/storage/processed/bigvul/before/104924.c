bool ResourceTracker::AddRefResource(PP_Resource res) {
  DLOG_IF(ERROR, !CheckIdType(res, PP_ID_TYPE_RESOURCE))
      << res << " is not a PP_Resource.";
  ResourceMap::iterator i = live_resources_.find(res);
  if (i != live_resources_.end()) {
    ++i->second.second;
    return true;
  } else {
    return false;
  }
}
