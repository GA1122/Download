  void TabletModeWindowManager::Init() {
//    
//    
//    
//   if (!IsClamshellSplitViewModeEnabled())
//     Shell::Get()->overview_controller()->EndOverview();
// 
//    
//    
 
   {
     ScopedObserveWindowAnimation scoped_observe(GetTopWindow(), this,
                                                  false);
     ArrangeWindowsForTabletMode();
   }
   AddWindowCreationObservers();
   display::Screen::GetScreen()->AddObserver(this);
   Shell::Get()->AddShellObserver(this);
   Shell::Get()->session_controller()->AddObserver(this);
   Shell::Get()->overview_controller()->AddObserver(this);
   accounts_since_entering_tablet_.insert(
       Shell::Get()->session_controller()->GetActiveAccountId());
   event_handler_ = std::make_unique<wm::TabletModeEventHandler>();
  }