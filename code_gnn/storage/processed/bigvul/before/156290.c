base::TickClock* RendererSchedulerImpl::tick_clock() const {
  return helper_.GetClock();
}
