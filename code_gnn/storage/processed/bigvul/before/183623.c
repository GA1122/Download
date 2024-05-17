 bool ChildProcessSecurityPolicy::CanRequestURL(
     int renderer_id, const GURL& url) {
   if (!url.is_valid())
     return false;   
 
   if (IsWebSafeScheme(url.scheme()))
     return true;   
 
   if (IsPseudoScheme(url.scheme())) {
 
      if (url.SchemeIs(chrome::kViewSourceScheme) ||
          url.SchemeIs(chrome::kPrintScheme)) {
      return CanRequestURL(renderer_id, GURL(url.path()));
//        
//       GURL child_url(url.path());
//       if (child_url.SchemeIs(chrome::kPrintScheme) ||
//           (child_url.SchemeIs(chrome::kViewSourceScheme) &&
//            url.SchemeIs(chrome::kViewSourceScheme)))
//           return false;
// 
//       return CanRequestURL(renderer_id, child_url);
      }
  
      if (LowerCaseEqualsASCII(url.spec(), chrome::kAboutBlankURL))
       return true;   
 
     return false;
   }
 
   if (!URLRequest::IsHandledURL(url))
     return true;   
 
   {
     AutoLock lock(lock_);
 
     SecurityStateMap::iterator state = security_state_.find(renderer_id);
     if (state == security_state_.end())
       return false;
 
     return state->second->CanRequestURL(url);
   }
 }