void FakeCrosDisksClient::Unmount(const std::string& device_path,
                                  UnmountOptions options,
                                  VoidDBusMethodCallback callback) {
  DCHECK(!callback.is_null());

  unmount_call_count_++;
  last_unmount_device_path_ = device_path;
  last_unmount_options_ = options;

  if (mounted_paths_.erase(base::FilePath::FromUTF8Unsafe(device_path))) {
    base::PostTaskWithTraitsAndReply(
        FROM_HERE,
        {base::MayBlock(), base::TaskPriority::BACKGROUND,
         base::TaskShutdownBehavior::CONTINUE_ON_SHUTDOWN},
        base::BindOnce(base::IgnoreResult(&base::DeleteFile),
                       base::FilePath::FromUTF8Unsafe(device_path),
                       true  ),
        base::BindOnce(std::move(callback), unmount_success_));
  } else {
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE, base::BindOnce(std::move(callback), unmount_success_));
  }
  if (!unmount_listener_.is_null())
    unmount_listener_.Run();
}
