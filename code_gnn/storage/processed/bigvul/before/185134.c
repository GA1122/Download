  bool ChromeContentBrowserClient::ShouldSwapProcessesForNavigation(
//     SiteInstance* site_instance,
      const GURL& current_url,
      const GURL& new_url) {
    if (current_url.is_empty()) {
     if (new_url.SchemeIs(extensions::kExtensionScheme))
       return true;
 
     return false;
   }
 
   if (current_url.SchemeIs(extensions::kExtensionScheme) ||
       new_url.SchemeIs(extensions::kExtensionScheme)) {
     if (current_url.GetOrigin() != new_url.GetOrigin())
        return true;
    }
  
//    
//    
//   Profile* profile =
//       Profile::FromBrowserContext(site_instance->GetBrowserContext());
//   ExtensionService* service =
//       extensions::ExtensionSystem::Get(profile)->extension_service();
//   if (!service)
//     return false;
// 
//    
//    
//   const Extension* new_extension =
//       service->extensions()->GetExtensionOrAppByURL(ExtensionURLInfo(new_url));
//    
//    
//   if (new_extension &&
//       new_extension->is_hosted_app() &&
//       new_extension->id() != extension_misc::kWebStoreAppId)
//     new_extension = NULL;
//   if (new_extension &&
//       site_instance->HasProcess() &&
//       !service->process_map()->Contains(new_extension->id(),
//                                         site_instance->GetProcess()->GetID()))
//     return true;
// 
    return false;
  }