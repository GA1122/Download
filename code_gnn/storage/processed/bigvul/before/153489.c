  TabHoverCardEventSniffer(TabHoverCardBubbleView* hover_card,
                           TabStrip* tab_strip)
      : hover_card_(hover_card),
        tab_strip_(tab_strip),
        widget_(tab_strip->GetWidget()) {
#if defined(OS_MACOSX)
    if (widget_->GetRootView())
      widget_->GetRootView()->AddPreTargetHandler(this);
#else
    if (widget_->GetNativeWindow())
      widget_->GetNativeWindow()->AddPreTargetHandler(this);
#endif
  }
