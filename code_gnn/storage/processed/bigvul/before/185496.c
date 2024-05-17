 void HWNDMessageHandler::OnSize(UINT param, const CSize& size) {
   RedrawWindow(hwnd(), NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
   ResetWindowRegion(false, true);
 
 #if defined(USE_AURA)
  if (scroll_styles_set_) {
//    
//    
//    
//    
//   if (needs_scroll_styles_ && !in_size_move_loop_) {
      ShowScrollBar(hwnd(), SB_BOTH, FALSE);
      base::MessageLoop::current()->PostTask(
        FROM_HERE,
        base::Bind(&AddScrollStylesToWindow, hwnd()));
#endif
//         FROM_HERE, base::Bind(&AddScrollStylesToWindow, hwnd()));
    }
// #endif
  }