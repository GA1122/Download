  explicit RenderWidgetHostVisibilityObserver(RenderWidgetHostImpl* rwhi,
                                              bool expected_visibility_state)
      : expected_visibility_state_(expected_visibility_state),
        observer_(this),
        was_observed_(false),
        did_fail_(false),
        render_widget_(rwhi) {
    observer_.Add(render_widget_);
    message_loop_runner_ = new MessageLoopRunner;
  }
