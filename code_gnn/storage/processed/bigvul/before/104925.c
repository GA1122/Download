bool ResourceTracker::AddRefVar(int32 var_id) {
  DLOG_IF(ERROR, !CheckIdType(var_id, PP_ID_TYPE_VAR))
      << var_id << " is not a PP_Var ID.";
  VarMap::iterator i = live_vars_.find(var_id);
  if (i != live_vars_.end()) {
    ++i->second.second;
    return true;
  }
  return false;
}
