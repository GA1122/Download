  SchedulerObject::release(std::string key, std::string &reason, std::string &text)
  {
         PROC_ID id = getProcByString(key.c_str());
       if (id.cluster < 0 || id.proc < 0) {
//        if (id.cluster <= 0 || id.proc < 0) {
                 dprintf(D_FULLDEBUG, "Release: Failed to parse id: %s\n", key.c_str());
                 text = "Invalid Id";
                 return false;
 	}
 
 	if (!releaseJob(id.cluster,
 					id.proc,
 					reason.c_str(),
 					true,  
 					false,  
 					false  
 					)) {
 		text = "Failed to release job";
 		return false;
 	}
 
 	return true;
 }