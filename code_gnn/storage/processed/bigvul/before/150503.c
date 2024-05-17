 SafeBrowsingPrivateEventRouter::SafeBrowsingPrivateEventRouter(
     content::BrowserContext* context)
    : context_(context) {
  event_router_ = EventRouter::Get(context_);
  InitRealtimeReportingClient();
}
