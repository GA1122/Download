  SchedulerObject::hold(std::string key, std::string &reason, std::string &text)
  {
         PROC_ID id = getProcByString(key.c_str());
       if (id.cluster < 0 || id.proc < 0) {
//        if (id.cluster <= 0 || id.proc < 0) {
                 dprintf(D_FULLDEBUG, "Hold: Failed to parse id: %s\n", key.c_str());
                 text = "Invalid Id";
                 return false;
 	}
 
 	if (!holdJob(id.cluster,
 				 id.proc,
 				 reason.c_str(),
 				 true,  
 				 true,  
 				 false,  
 				 false,  
 				 false  
 				 )) {
 		text = "Failed to hold job";
 		return false;
 	}
 
 	return true;
 }