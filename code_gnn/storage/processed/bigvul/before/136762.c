  PostMessageTimer(LocalDOMWindow& window,
                   MessageEvent* event,
                   scoped_refptr<const SecurityOrigin> target_origin,
                   std::unique_ptr<SourceLocation> location,
                   UserGestureToken* user_gesture_token)
      : PausableTimer(window.document(), TaskType::kPostedMessage),
        event_(event),
        window_(&window),
        target_origin_(std::move(target_origin)),
        location_(std::move(location)),
        user_gesture_token_(user_gesture_token),
        disposal_allowed_(true) {
  }
