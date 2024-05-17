  explicit MountState(DriveFsHost* host)
      : host_(host),
        mojo_connection_delegate_(
            host_->delegate_->CreateMojoConnectionDelegate()),
        pending_token_(base::UnguessableToken::Create()),
        binding_(this) {
    host_->disk_mount_manager_->AddObserver(this);
    source_path_ = base::StrCat({kMountScheme, pending_token_.ToString()});
    std::string datadir_option =
        base::StrCat({"datadir=", host_->GetDataPath().value()});
    auto bootstrap =
        mojo::MakeProxy(mojo_connection_delegate_->InitializeMojoConnection());
    mojom::DriveFsDelegatePtr delegate;
    binding_.Bind(mojo::MakeRequest(&delegate));
    binding_.set_connection_error_handler(
        base::BindOnce(&MountState::OnConnectionError, base::Unretained(this)));
    bootstrap->Init(
        {base::in_place, host_->delegate_->GetAccountId().GetUserEmail()},
        mojo::MakeRequest(&drivefs_), std::move(delegate));
    drivefs_.set_connection_error_handler(
        base::BindOnce(&MountState::OnConnectionError, base::Unretained(this)));

    PendingConnectionManager::Get().ExpectOpenIpcChannel(
        pending_token_,
        base::BindOnce(&DriveFsHost::MountState::AcceptMojoConnection,
                       base::Unretained(this)));

    host_->disk_mount_manager_->MountPath(
        source_path_, "",
        base::StrCat({"drivefs-", host_->delegate_->GetObfuscatedAccountId()}),
        {datadir_option}, chromeos::MOUNT_TYPE_NETWORK_STORAGE,
        chromeos::MOUNT_ACCESS_MODE_READ_WRITE);

    host_->timer_->Start(
        FROM_HERE, kMountTimeout,
        base::BindOnce(&MountState::OnTimedOut, base::Unretained(this)));
  }
