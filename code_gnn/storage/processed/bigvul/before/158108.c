void LocalFrameClientImpl::NotifyUserActivation() {
  DCHECK(web_frame_->Client());
  web_frame_->Client()->UpdateUserActivationState(
      UserActivationUpdateType::kNotifyActivation);
  if (WebAutofillClient* autofill_client = web_frame_->AutofillClient())
    autofill_client->UserGestureObserved();
}
