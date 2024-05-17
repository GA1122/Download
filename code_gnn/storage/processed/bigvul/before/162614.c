PrintRenderFrameHelper::PrintRenderFrameHelper(
    content::RenderFrame* render_frame,
    std::unique_ptr<Delegate> delegate)
    : content::RenderFrameObserver(render_frame),
      content::RenderFrameObserverTracker<PrintRenderFrameHelper>(render_frame),
      reset_prep_frame_view_(false),
      is_print_ready_metafile_sent_(false),
      ignore_css_margins_(false),
      is_printing_enabled_(true),
      notify_browser_of_print_failure_(true),
      print_for_preview_(false),
      delegate_(std::move(delegate)),
      print_node_in_progress_(false),
      is_loading_(false),
      is_scripted_preview_delayed_(false),
      ipc_nesting_level_(0),
      render_frame_gone_(false),
      weak_ptr_factory_(this) {
  if (!delegate_->IsPrintPreviewEnabled())
    DisablePreview();
}
