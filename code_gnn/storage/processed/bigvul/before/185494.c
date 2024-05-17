  void HWNDMessageHandler::OnEnterSizeMove() {
//   in_size_move_loop_ = true;
// 
//    
//    
//    
//    
//   if (needs_scroll_styles_)
//     ShowScrollBar(hwnd(), SB_BOTH, FALSE);
// 
    delegate_->HandleBeginWMSizeMove();
    SetMsgHandled(FALSE);
  }