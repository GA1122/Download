PrintWebViewHelper::PrintWebViewHelper(RenderView* render_view)
    : render_view_(render_view),
      print_web_view_(NULL),
      user_cancelled_scripted_print_count_(0),
      is_preview_(false) {}
