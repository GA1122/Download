RenderMediaClient::RenderMediaClient()
    : has_updated_(false),
      is_update_needed_(true),
      tick_clock_(new base::DefaultTickClock()) {
  media::SetMediaClient(this);
}
