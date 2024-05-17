ExtensionSystemImpl::ExtensionSystemImpl(Profile* profile)
    : profile_(profile) {
  shared_ = ExtensionSystemSharedFactory::GetForProfile(profile);

  if (profile->IsOffTheRecord()) {
    extension_process_manager_.reset(ExtensionProcessManager::Create(profile));
  } else {
    shared_->InitPrefs();
  }
}
