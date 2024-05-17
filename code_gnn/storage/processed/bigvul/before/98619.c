void ThumbnailGenerator::StartThumbnailing() {
  if (registrar_.IsEmpty()) {
    registrar_.Add(this, NotificationType::RENDER_VIEW_HOST_CREATED_FOR_TAB,
                   NotificationService::AllSources());
    registrar_.Add(this, NotificationType::RENDER_WIDGET_VISIBILITY_CHANGED,
                   NotificationService::AllSources());
    registrar_.Add(this, NotificationType::RENDER_WIDGET_HOST_DESTROYED,
                   NotificationService::AllSources());
    registrar_.Add(this, NotificationType::TAB_CONTENTS_DISCONNECTED,
                   NotificationService::AllSources());
  }
}
