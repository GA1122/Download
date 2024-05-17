  void Click() {
    if (is_hosted_app_) {
      BackgroundContentsService* service =
          BackgroundContentsServiceFactory::GetForProfile(profile_);
      if (!service->GetAppBackgroundContents(ASCIIToUTF16(extension_id_)))
        service->LoadBackgroundContentsForExtension(profile_, extension_id_);
    } else {
      profile_->GetExtensionService()->ReloadExtension(extension_id_);
    }

    ScheduleCloseBalloon(extension_id_);
  }
