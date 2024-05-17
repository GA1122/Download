  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) {
    DCHECK(type == content::NOTIFICATION_DOM_OPERATION_RESPONSE);
    content::Details<DomOperationNotificationDetails> dom_op_details(details);
    std::string response;
    TrimString(dom_op_details->json, "\"", &response);
    if (response == "...") {
      timer_.Stop();
      timer_.Start(FROM_HERE, base::TimeDelta::FromSeconds(timeout_s_),
                   this, &TestFinishObserver::OnTimeout);
    } else {
      result_ = response;
      finished_ = true;
      if (waiting_)
        MessageLoopForUI::current()->Quit();
    }
  }
