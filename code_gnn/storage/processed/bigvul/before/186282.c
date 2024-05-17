 void PrintViewManager::OnShowScriptedPrintPreview(content::RenderFrameHost* rfh,
                                                   bool source_is_modifiable) {
   DCHECK(print_preview_rfh_);
   if (rfh != print_preview_rfh_)
     return;
 
   PrintPreviewDialogController* dialog_controller =
       PrintPreviewDialogController::GetInstance();
   if (!dialog_controller) {
     PrintPreviewDone();
      return;
    }
  
//    
//    
//   if (web_contents()->IsFullscreenForCurrentTab())
//     web_contents()->ExitFullscreen(true);
// 
    dialog_controller->PrintPreview(web_contents());
    PrintHostMsg_RequestPrintPreview_Params params;
    params.is_modifiable = source_is_modifiable;
   PrintPreviewUI::SetInitialParams(
       dialog_controller->GetPrintPreviewForContents(web_contents()), params);
 }