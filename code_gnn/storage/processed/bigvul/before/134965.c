void FakeCrosDisksClient::DidMount(const std::string& source_path,
                                   MountType type,
                                   const base::FilePath& mounted_path,
                                   VoidDBusMethodCallback callback,
                                   MountError mount_error) {
  std::move(callback).Run(true);

  NotifyMountCompleted(mount_error, source_path, type,
                       mounted_path.AsUTF8Unsafe());
}
