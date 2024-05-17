void WebMediaPlayerImpl::SetCdm(blink::WebContentDecryptionModule* cdm) {
  DCHECK(main_task_runner_->BelongsToCurrentThread());
  DCHECK(cdm);

  auto cdm_context_ref =
      ToWebContentDecryptionModuleImpl(cdm)->GetCdmContextRef();
  if (!cdm_context_ref) {
    NOTREACHED();
    OnCdmAttached(false);
    return;
  }

  CdmContext* cdm_context = cdm_context_ref->GetCdmContext();
  DCHECK(cdm_context);

  pending_cdm_context_ref_ = std::move(cdm_context_ref);
  pipeline_controller_.SetCdm(
      cdm_context, base::Bind(&WebMediaPlayerImpl::OnCdmAttached, AsWeakPtr()));
}
