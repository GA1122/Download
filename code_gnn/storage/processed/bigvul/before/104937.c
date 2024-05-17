scoped_refptr<Var> ResourceTracker::GetVar(int32 var_id) const {
  DLOG_IF(ERROR, !CheckIdType(var_id, PP_ID_TYPE_VAR))
      << var_id << " is not a PP_Var ID.";
  VarMap::const_iterator result = live_vars_.find(var_id);
  if (result == live_vars_.end())
    return scoped_refptr<Var>();
  return result->second.first;
}
