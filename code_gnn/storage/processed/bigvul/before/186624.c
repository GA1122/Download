  InputImeEventRouter* InputImeEventRouterFactory::GetRouter(Profile* profile) {
    if (!profile)
      return nullptr;
//    
//    
//   profile = profile->GetOriginalProfile();
    InputImeEventRouter* router = router_map_[profile];
    if (!router) {
    router = new InputImeEventRouter(profile);
//      
//      
//      
//      
//     router = new InputImeEventRouter(profile->HasOffTheRecordProfile()
//                                          ? profile->GetOffTheRecordProfile()
//                                          : profile);
      router_map_[profile] = router;
    }
    return router;
 }