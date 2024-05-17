bool ResourceTracker::UnrefVar(int32 var_id) {
  DLOG_IF(ERROR, !CheckIdType(var_id, PP_ID_TYPE_VAR))
      << var_id << " is not a PP_Var ID.";
  VarMap::iterator i = live_vars_.find(var_id);
  if (i != live_vars_.end()) {
    if (!--i->second.second) {
      ObjectVar* object_var = i->second.first->AsObjectVar();
      if (object_var) {
        instance_map_[object_var->instance()->pp_instance()]->object_vars.erase(
            var_id);
      }
      live_vars_.erase(i);
    }
    return true;
  }
  return false;
}
