void OzoneRegisterStartupCallbackHelper(
    base::OnceCallback<void(ui::OzonePlatform*)> io_callback) {
  auto bounce_callback = base::BindOnce(
      [](base::TaskRunner* task_runner,
         base::OnceCallback<void(ui::OzonePlatform*)> callback,
         ui::OzonePlatform* platform) {
        if (task_runner->RunsTasksInCurrentSequence()) {
          std::move(callback).Run(platform);
        } else {
          task_runner->PostTask(FROM_HERE,
                                base::BindOnce(std::move(callback), platform));
        }
      },
      base::RetainedRef(base::ThreadTaskRunnerHandle::Get()),
      std::move(io_callback));
  ui::OzonePlatform::RegisterStartupCallback(std::move(bounce_callback));
}
