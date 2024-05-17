void RenderWidgetHostImpl::DidNotProduceFrame(const viz::BeginFrameAck& ack) {
  viz::BeginFrameAck modified_ack = ack;
  modified_ack.has_damage = false;

  if (view_)
    view_->OnDidNotProduceFrame(modified_ack);
}
