BaseAudioContext::BaseAudioContext(Document* document,
                                   enum ContextType context_type)
    : PausableObject(document),
      destination_node_(nullptr),
      is_cleared_(false),
      is_resolving_resume_promises_(false),
      has_posted_cleanup_task_(false),
      user_gesture_required_(false),
      connection_count_(0),
      deferred_task_handler_(DeferredTaskHandler::Create()),
      context_state_(kSuspended),
      closed_context_sample_rate_(-1),
      periodic_wave_sine_(nullptr),
      periodic_wave_square_(nullptr),
      periodic_wave_sawtooth_(nullptr),
      periodic_wave_triangle_(nullptr),
      output_position_() {
  switch (context_type) {
    case kRealtimeContext:
      switch (GetAutoplayPolicy()) {
        case AutoplayPolicy::Type::kNoUserGestureRequired:
          break;
        case AutoplayPolicy::Type::kUserGestureRequired:
        case AutoplayPolicy::Type::kUserGestureRequiredForCrossOrigin:
          if (document->GetFrame() &&
              document->GetFrame()->IsCrossOriginSubframe()) {
            autoplay_status_ = AutoplayStatus::kAutoplayStatusFailed;
            user_gesture_required_ = true;
          }
          break;
        case AutoplayPolicy::Type::kDocumentUserActivationRequired:
          autoplay_status_ = AutoplayStatus::kAutoplayStatusFailed;
          user_gesture_required_ = true;
          break;
      }
      break;
    case kOfflineContext:
      break;
    default:
      NOTREACHED();
      break;
  }
}
