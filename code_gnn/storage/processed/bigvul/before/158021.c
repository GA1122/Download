void LocalFrameClientImpl::ConsumeUserActivation() {
  DCHECK(web_frame_->Client());
  web_frame_->Client()->UpdateUserActivationState(
      UserActivationUpdateType::kConsumeTransientActivation);
}
