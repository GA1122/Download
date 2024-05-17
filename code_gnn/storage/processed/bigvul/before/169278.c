void ContextMenuFilter::OnContextMenu(
    const content::ContextMenuParams& params) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  handled_ = true;
  last_params_ = params;
  message_loop_runner_->Quit();
}
