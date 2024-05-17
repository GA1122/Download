   SessionRestoreImpl(Profile* profile,
                      Browser* browser,
                      bool synchronous,
                      bool clobber_existing_tab,
                      bool always_create_tabbed_browser,
                      const std::vector<GURL>& urls_to_open)
       : profile_(profile),
         browser_(browser),
         synchronous_(synchronous),
         clobber_existing_tab_(clobber_existing_tab),
         always_create_tabbed_browser_(always_create_tabbed_browser),
          urls_to_open_(urls_to_open),
          restore_started_(base::TimeTicks::Now()),
          browser_shown_(false) {
    if (profiles_getting_restored == NULL)
      profiles_getting_restored = new std::set<const Profile*>();
  
    CHECK(profiles_getting_restored->find(profile) ==
          profiles_getting_restored->end());
//      
//      
//      
//     if (active_session_restorers == NULL)
//       active_session_restorers = new std::set<SessionRestoreImpl*>();
// 
//     std::set<SessionRestoreImpl*>::const_iterator it;
//     for (it = active_session_restorers->begin();
//          it != active_session_restorers->end(); ++it) {
//       if ((*it)->profile_ == profile)
//         break;
//     }
//     DCHECK(it == active_session_restorers->end());
// 
//     active_session_restorers->insert(this);
  
    profiles_getting_restored->insert(profile);
     g_browser_process->AddRefModule();
   }