void FakeCrosDisksClient::Rename(const std::string& device_path,
                                 const std::string& volume_name,
                                 VoidDBusMethodCallback callback) {
  DCHECK(!callback.is_null());

  rename_call_count_++;
  last_rename_device_path_ = device_path;
  last_rename_volume_name_ = volume_name;
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::BindOnce(std::move(callback), rename_success_));
}
