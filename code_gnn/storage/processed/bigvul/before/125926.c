void OnNotificationBalloonCountObserver::CheckBalloonCount() {
  bool balloon_count_met = AreActiveNotificationProcessesReady() &&
      static_cast<int>(collection_->GetActiveBalloons().size()) == count_;

  if (balloon_count_met && automation_) {
    AutomationJSONReply(automation_,
                        reply_message_.release()).SendSuccess(NULL);
  }

  if (balloon_count_met || !automation_) {
    collection_->set_on_collection_changed_callback(base::Closure());
    delete this;
  }
}
