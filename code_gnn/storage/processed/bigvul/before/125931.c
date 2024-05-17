void NavigationNotificationObserver::ConditionMet(
    AutomationMsg_NavigationResponseValues navigation_result) {
  if (automation_) {
    if (use_json_interface_) {
      if (navigation_result == AUTOMATION_MSG_NAVIGATION_SUCCESS) {
        DictionaryValue dict;
        dict.SetInteger("result", navigation_result);
        AutomationJSONReply(automation_, reply_message_.release()).SendSuccess(
            &dict);
      } else {
        AutomationJSONReply(automation_, reply_message_.release()).SendError(
            StringPrintf("Navigation failed with error code=%d.",
                         navigation_result));
      }
    } else {
      IPC::ParamTraits<int>::Write(
          reply_message_.get(), navigation_result);
      automation_->Send(reply_message_.release());
    }
  }

  delete this;
}
