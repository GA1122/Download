void RenderMediaClient::SetTickClockForTesting(
    std::unique_ptr<base::TickClock> tick_clock) {
  tick_clock_.swap(tick_clock);
}
