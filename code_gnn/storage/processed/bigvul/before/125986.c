void FindInPageNotificationObserver::Observe(
    int type, const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  content::Details<FindNotificationDetails> find_details(details);
  if (!(find_details->final_update() && reply_message_ != NULL)) {
    DVLOG(1) << "Ignoring, since we only care about the final message";
    return;
  }

  if (!automation_) {
    delete this;
    return;
  }

  if (find_details->request_id() == kFindInPageRequestId) {
    if (reply_with_json_) {
      scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
      return_value->SetInteger("match_count",
          find_details->number_of_matches());
      gfx::Rect rect = find_details->selection_rect();
      if (!rect.IsEmpty()) {
        return_value->SetInteger("match_left", rect.x());
        return_value->SetInteger("match_top", rect.y());
        return_value->SetInteger("match_right", rect.right());
        return_value->SetInteger("match_bottom", rect.bottom());
      }
      AutomationJSONReply(automation_, reply_message_.release())
          .SendSuccess(return_value.get());
      delete this;
    } else {
      if (find_details->active_match_ordinal() > -1) {
        active_match_ordinal_ = find_details->active_match_ordinal();
        AutomationMsg_Find::WriteReplyParams(reply_message_.get(),
            active_match_ordinal_, find_details->number_of_matches());
        automation_->Send(reply_message_.release());
      }
    }
  }
}
