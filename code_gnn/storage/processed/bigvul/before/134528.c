  explicit OverscrollNavigationOverlay(WebContentsImpl* web_contents)
      : web_contents_(web_contents),
        image_delegate_(NULL),
        view_(NULL),
        loading_complete_(false),
        received_paint_update_(false),
        compositor_updated_(false),
        slide_direction_(SLIDE_UNKNOWN),
        need_paint_update_(true) {
  }
