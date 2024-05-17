 void TabletModeWindowState::LeaveTabletMode(wm::WindowState* window_state) {
// void TabletModeWindowState::LeaveTabletMode(wm::WindowState* window_state,
//                                             bool was_in_overview) {
//    
//    
//    
//    
    EnterAnimationType animation_type =
      window_state->IsSnapped() || IsTopWindow(window_state->window())
//       was_in_overview || window_state->IsSnapped() ||
//               IsTopWindow(window_state->window())
            ? DEFAULT
            : IMMEDIATE;
    if (old_state_->GetType() == window_state->GetStateType() &&
       !window_state->IsNormalStateType()) {
     animation_type = IMMEDIATE;
   }
   old_state_->set_enter_animation_type(animation_type);
   std::unique_ptr<wm::WindowState::State> our_reference =
       window_state->SetStateObject(std::move(old_state_));
 }