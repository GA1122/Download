  bool ChildProcessSecurityPolicyImpl::CanCommitURL(int child_id,
                                                  const GURL& url) {
//                                                   const GURL& url,
//                                                   bool check_origin_locks) {
    if (!url.is_valid())
      return false;   
  
   const std::string& scheme = url.scheme();
 
   if (IsPseudoScheme(scheme))
     return url == url::kAboutBlankURL || url == kAboutSrcDocURL;
 
   if (url.SchemeIsBlob() || url.SchemeIsFileSystem()) {
     if (IsMalformedBlobUrl(url))
        return false;
  
      url::Origin origin = url::Origin::Create(url);
    return origin.unique() || CanCommitURL(child_id, GURL(origin.Serialize()));
//     return origin.unique() ||
//            CanCommitURL(child_id, GURL(origin.Serialize()), check_origin_locks);
    }
  
//    
//    
//    
//    
//    
//    
//   if (check_origin_locks && !CanAccessDataForOrigin(child_id, url))
//     return false;
// 
    {
      base::AutoLock lock(lock_);
  
//      
      if (base::ContainsKey(schemes_okay_to_commit_in_any_process_, scheme))
        return true;
 
     SecurityStateMap::iterator state = security_state_.find(child_id);
     if (state == security_state_.end())
       return false;
 
     return state->second->CanCommitURL(url);
    }
  }