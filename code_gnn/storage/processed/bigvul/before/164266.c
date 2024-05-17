  void SendNotifications(blink::mojom::AppCacheEventID event_id) {
    for (auto* frontend : hosts_to_notify_)
      frontend->EventRaised(event_id);
  }
