WallpaperManager::WallpaperManager()
    : binding_(this),
      pending_inactive_(nullptr),
      activation_client_observer_(this),
      window_observer_(this),
      weak_factory_(this) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  wallpaper::WallpaperManagerBase::SetPathIds(
      chrome::DIR_USER_DATA, chrome::DIR_CHROMEOS_WALLPAPERS,
      chrome::DIR_CHROMEOS_CUSTOM_WALLPAPERS);
  SetDefaultWallpaperPathsFromCommandLine(
      base::CommandLine::ForCurrentProcess());
  registrar_.Add(this, chrome::NOTIFICATION_LOGIN_USER_CHANGED,
                 content::NotificationService::AllSources());
  registrar_.Add(this, chrome::NOTIFICATION_LOGIN_OR_LOCK_WEBUI_VISIBLE,
                 content::NotificationService::AllSources());
  registrar_.Add(this, chrome::NOTIFICATION_WALLPAPER_ANIMATION_FINISHED,
                 content::NotificationService::AllSources());
  task_runner_ = base::CreateSequencedTaskRunnerWithTraits(
      {base::MayBlock(), base::TaskPriority::USER_BLOCKING,
       base::TaskShutdownBehavior::CONTINUE_ON_SHUTDOWN});

  user_manager::UserManager::Get()->AddSessionStateObserver(this);

  content::ServiceManagerConnection* connection =
      content::ServiceManagerConnection::GetForProcess();
  if (connection && connection->GetConnector()) {
    ash::mojom::WallpaperControllerPtr wallpaper_controller_ptr;
    connection->GetConnector()->BindInterface(ash::mojom::kServiceName,
                                              &wallpaper_controller_ptr);
    ash::mojom::WallpaperPickerPtr picker;
    binding_.Bind(mojo::MakeRequest(&picker));
    wallpaper_controller_ptr->SetWallpaperPicker(std::move(picker));
  }
}
