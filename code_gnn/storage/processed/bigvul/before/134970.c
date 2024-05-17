void FakeCrosDisksClient::Format(const std::string& device_path,
                                 const std::string& filesystem,
                                 VoidDBusMethodCallback callback) {
  DCHECK(!callback.is_null());

  format_call_count_++;
  last_format_device_path_ = device_path;
  last_format_filesystem_ = filesystem;
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::BindOnce(std::move(callback), format_success_));
}
