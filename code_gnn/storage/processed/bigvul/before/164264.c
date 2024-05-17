  void SendErrorNotifications(
      const blink::mojom::AppCacheErrorDetails& details) {
    DCHECK(!details.message.empty());
    for (auto* frontend : hosts_to_notify_)
      frontend->ErrorEventRaised(details.Clone());
  }
