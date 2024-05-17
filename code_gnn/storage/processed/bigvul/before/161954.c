bool PrintRenderFrameHelper::RenderPagesForPrint(blink::WebLocalFrame* frame,
                                                 const blink::WebNode& node) {
  if (!frame || prep_frame_view_)
    return false;

  const PrintMsg_PrintPages_Params& params = *print_pages_params_;
  const PrintMsg_Print_Params& print_params = params.params;
  prep_frame_view_ = base::MakeUnique<PrepareFrameAndViewForPrint>(
      print_params, frame, node, ignore_css_margins_);
  DCHECK(!print_pages_params_->params.selection_only ||
         print_pages_params_->pages.empty());
  prep_frame_view_->CopySelectionIfNeeded(
      render_frame()->GetWebkitPreferences(),
      base::Bind(&PrintRenderFrameHelper::OnFramePreparedForPrintPages,
                 weak_ptr_factory_.GetWeakPtr()));
  return true;
}
