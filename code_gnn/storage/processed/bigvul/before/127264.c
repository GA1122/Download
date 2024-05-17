WtsSessionProcessDelegate::WtsSessionProcessDelegate(
    scoped_refptr<base::SingleThreadTaskRunner> main_task_runner,
    scoped_refptr<base::SingleThreadTaskRunner> io_task_runner,
    const FilePath& binary_path,
    uint32 session_id,
    bool launch_elevated,
    const std::string& channel_security) {
  core_ = new Core(main_task_runner, io_task_runner, binary_path,
                   launch_elevated, channel_security);
  if (!core_->Initialize(session_id)) {
    core_->Stop();
    core_ = NULL;
  }
}
