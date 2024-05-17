void ContextualSearchDelegate::GatherSurroundingTextWithCallback(
    const std::string& selection,
    bool use_resolved_search_term,
    content::WebContents* web_contents,
    bool may_send_base_page_url,
    HandleSurroundingsCallback callback) {
  DCHECK(web_contents);
  DCHECK(!callback.is_null());
  DCHECK(!selection.empty());
  RenderFrameHost* focused_frame = web_contents->GetFocusedFrame();
  if (!focused_frame) {
    callback.Run(base::string16(), 0, 0);
    return;
  }
  search_term_fetcher_.reset();
  BuildContext(selection, use_resolved_search_term, web_contents,
               may_send_base_page_url);
  focused_frame->RequestTextSurroundingSelection(
      callback, field_trial_->GetSurroundingSize());
}
