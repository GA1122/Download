void TabCountChangeObserver::TabStripModelDeleted() {
  if (automation_) {
    AutomationMsg_WaitForTabCountToBecome::WriteReplyParams(
        reply_message_.get(), false);
    automation_->Send(reply_message_.release());
  }

  delete this;
}
