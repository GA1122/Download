ScrollAnchor::ScrollAnchor()
    : anchor_object_(nullptr),
      corner_(Corner::kTopLeft),
      scroll_anchor_disabling_style_changed_(false),
      queued_(false) {}
