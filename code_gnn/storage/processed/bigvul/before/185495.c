  void HWNDMessageHandler::OnExitSizeMove() {
    delegate_->HandleEndWMSizeMove();
    SetMsgHandled(FALSE);
//   in_size_move_loop_ = false;
//    
//    
//    
//    
//    
//   if (needs_scroll_styles_)
//     AddScrollStylesToWindow(hwnd());
  }