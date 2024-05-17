 bool SiteInstanceImpl::DoesSiteRequireDedicatedProcess(
     BrowserContext* browser_context,
     const GURL& url) {
    if (SiteIsolationPolicy::UseDedicatedProcessesForAllSites())
      return true;
  
  if (url.SchemeIs(kChromeErrorScheme))
     return true;
 
//    
   GURL site_url = SiteInstance::GetSiteForURL(browser_context, url);
   auto* policy = ChildProcessSecurityPolicyImpl::GetInstance();
   if (policy->IsIsolatedOrigin(url::Origin::Create(site_url)))
//     return true;
// 
//   if (site_url.SchemeIs(kChromeErrorScheme))
//      return true;
//  
//    
//    
//   if (site_url.SchemeIs(content::kChromeUIScheme))
      return true;
  
   if (GetContentClient()->browser()->DoesSiteRequireDedicatedProcess(
           browser_context, site_url)) {
     return true;
   }
 
   return false;
 }