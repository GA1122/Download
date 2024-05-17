void LocalFrame::ScheduleNavigation(Document& origin_document,
                                    const KURL& url,
                                    bool replace_current_item,
                                    UserGestureStatus user_gesture_status) {
  navigation_scheduler_->ScheduleFrameNavigation(&origin_document, url,
                                                 replace_current_item);
}
