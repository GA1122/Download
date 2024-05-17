bool MessageService::MaybeAddPendingOpenChannelTask(
    Profile* profile,
    OpenChannelParams* params) {
  ExtensionService* service = profile->GetExtensionService();
  const std::string& extension_id = params->target_extension_id;
  const Extension* extension = service->extensions()->GetByID(extension_id);
  if (extension && extension->has_lazy_background_page()) {
    if (!extension->incognito_split_mode())
      profile = profile->GetOriginalProfile();

    if (lazy_background_task_queue_->ShouldEnqueueTask(profile, extension)) {
      pending_channels_[GET_CHANNEL_ID(params->receiver_port_id)] =
          PendingChannel(profile, extension_id);
      scoped_ptr<OpenChannelParams> scoped_params(params);
      lazy_background_task_queue_->AddPendingTask(profile, extension_id,
          base::Bind(&MessageService::PendingOpenChannel,
                     weak_factory_.GetWeakPtr(), base::Passed(&scoped_params),
                     params->source->GetID()));
      return true;
    }
  }

  return false;
}
