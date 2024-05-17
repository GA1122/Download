void ChromeClientImpl::DidAssociateFormControlsAfterLoad(LocalFrame* frame) {
  if (auto* fill_client = AutofillClientFromFrame(frame))
    fill_client->DidAssociateFormControlsDynamically();
}
