  void Add(RenderWidgetHost* host) {
    hosts_.push_back(RenderWidgetHostID(host->GetProcess()->GetID(),
                                        host->GetRoutingID()));
  }
