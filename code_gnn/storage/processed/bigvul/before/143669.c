void RenderWidgetHostImpl::OnQueueSyntheticGesture(
    const SyntheticGesturePacket& gesture_packet) {
  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
          cc::switches::kEnableGpuBenchmarking)) {
    bad_message::ReceivedBadMessage(GetProcess(),
                                    bad_message::RWH_SYNTHETIC_GESTURE);
    return;
  }

  QueueSyntheticGesture(
        SyntheticGesture::Create(*gesture_packet.gesture_params()),
        base::Bind(&RenderWidgetHostImpl::OnSyntheticGestureCompleted,
                   weak_factory_.GetWeakPtr()));
}
