  void Update(IPC::Channel::Sender* sender) {
    DCHECK(sender);

    if (!has_receiver_)
      return;

    if (reset_needed_) {
      sender->Send(new ViewMsg_VisitedLink_Reset());
      reset_needed_ = false;
      return;
    }

    if (pending_.size() == 0)
      return;

    sender->Send(new ViewMsg_VisitedLink_Add(pending_));

    pending_.clear();
  }
