void TabCountChangeObserver::CheckTabCount() {
  if (tab_strip_model_->count() != target_tab_count_)
    return;

  if (automation_) {
    AutomationMsg_WaitForTabCountToBecome::WriteReplyParams(
        reply_message_.get(), true);
    automation_->Send(reply_message_.release());
  }

  delete this;
}
