 void ResourceTracker::CleanupInstanceData(PP_Instance instance,
                                           bool delete_instance) {
   DLOG_IF(ERROR, !CheckIdType(instance, PP_ID_TYPE_INSTANCE))
       << instance << " is not a PP_Instance.";
   InstanceMap::iterator found = instance_map_.find(instance);
   if (found == instance_map_.end()) {
     NOTREACHED();
     return;
   }
   InstanceData& data = *found->second;
  
  ResourceSet::iterator cur_res = data.resources.begin();
  while (cur_res != data.resources.end()) {
//   ResourceSet::iterator cur_res = data.ref_resources.begin();
//   while (cur_res != data.ref_resources.end()) {
      ResourceMap::iterator found_resource = live_resources_.find(*cur_res);
      if (found_resource == live_resources_.end()) {
        NOTREACHED();
     } else {
       Resource* resource = found_resource->second.first;
  
      resource->LastPluginRefWasDeleted(true);
//       resource->LastPluginRefWasDeleted();
        live_resources_.erase(*cur_res);
      }
  
      ResourceSet::iterator current = cur_res++;
    data.resources.erase(current);
//     data.ref_resources.erase(current);
    }
  DCHECK(data.resources.empty());
//   DCHECK(data.ref_resources.empty());
  
    VarSet::iterator cur_var = data.object_vars.begin();
   while (cur_var != data.object_vars.end()) {
     VarSet::iterator current = cur_var++;
 
     PP_Var object_pp_var;
     object_pp_var.type = PP_VARTYPE_OBJECT;
     object_pp_var.value.as_id = *current;
     scoped_refptr<ObjectVar> object_var(ObjectVar::FromPPVar(object_pp_var));
     if (object_var.get())
       object_var->InstanceDeleted();
 
     live_vars_.erase(*current);
     data.object_vars.erase(*current);
    }
    DCHECK(data.object_vars.empty());
  
//    
//   for (std::set<Resource*>::iterator res = data.assoc_resources.begin();
//        res != data.assoc_resources.end();
//        ++res)
//     (*res)->ClearInstance();
//   data.assoc_resources.clear();
// 
    if (delete_instance)
      instance_map_.erase(found);
  }