 void VRDisplay::ProcessScheduledWindowAnimations(double timestamp) {
   TRACE_EVENT1("gpu", "VRDisplay::window.rAF", "frame", vr_frame_id_);
   auto doc = navigator_vr_->GetDocument();
   if (!doc)
     return;
    auto page = doc->GetPage();
    if (!page)
      return;
// 
//   bool had_pending_vrdisplay_raf = pending_vrdisplay_raf_;
    page->Animator().ServiceScriptedAnimations(timestamp);
// 
//   if (had_pending_vrdisplay_raf != pending_vrdisplay_raf_) {
//     DVLOG(1) << __FUNCTION__
//              << ": window.rAF fallback successfully scheduled VRDisplay.rAF";
//   }
// 
//   if (!pending_vrdisplay_raf_) {
//      
//      
//      
//      
//     DVLOG(1) << __FUNCTION__
//              << ": no scheduled VRDisplay.requestAnimationFrame, presentation "
//                 "broken?";
//   }
  }