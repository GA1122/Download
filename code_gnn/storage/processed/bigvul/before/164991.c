void HTMLCanvasElement::DisableDeferral(DisableDeferralReason reason) {
  if (canvas2d_bridge_)
    canvas2d_bridge_->DisableDeferral(reason);
}
