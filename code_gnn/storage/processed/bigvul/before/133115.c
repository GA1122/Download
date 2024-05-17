void HWNDMessageHandler::OnEnterMenuLoop(BOOL from_track_popup_menu) {
  if (menu_depth_++ == 0)
    delegate_->HandleMenuLoop(true);
 }
