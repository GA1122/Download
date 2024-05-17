PrepareFrameAndViewForPrint::PrepareFrameAndViewForPrint(
    const PrintMsg_Print_Params& print_params,
    WebFrame* frame,
    WebNode* node)
        : frame_(frame),
          web_view_(frame->view()),
          dpi_(static_cast<int>(print_params.dpi)),
          expected_pages_count_(0),
          use_browser_overlays_(true),
          finished_(false) {
  gfx::Size canvas_size;
  CalculatePrintCanvasSize(print_params, &canvas_size);

  if (WebFrame* web_frame = web_view_->mainFrame())
    prev_scroll_offset_ = web_frame->scrollOffset();
  prev_view_size_ = web_view_->size();

  if (node)
    node_to_print_ = *node;

  StartPrinting(canvas_size);
}
