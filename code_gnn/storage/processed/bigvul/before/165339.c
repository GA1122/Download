void DOMStorageContextWrapper::GetLocalStorageUsage(
    GetLocalStorageUsageCallback callback) {
  DCHECK(context_.get());
  auto infos = std::make_unique<std::vector<StorageUsageInfo>>();
  auto* infos_ptr = infos.get();
  base::RepeatingClosure got_local_storage_usage = base::BarrierClosure(
      2, base::BindOnce(&InvokeLocalStorageUsageCallbackHelper,
                        std::move(callback), std::move(infos)));
  auto collect_callback = base::BindRepeating(
      CollectLocalStorageUsage, infos_ptr, std::move(got_local_storage_usage));
  mojo_task_runner_->PostTask(
      FROM_HERE,
      base::BindOnce(&LocalStorageContextMojo::GetStorageUsage,
                     base::Unretained(mojo_state_),
                     base::BindOnce(&GotMojoLocalStorageUsage,
                                    base::ThreadTaskRunnerHandle::Get(),
                                    collect_callback)));
  context_->task_runner()->PostShutdownBlockingTask(
      FROM_HERE, DOMStorageTaskRunner::PRIMARY_SEQUENCE,
      base::BindOnce(&GetLegacyLocalStorageUsage, legacy_localstorage_path_,
                     base::ThreadTaskRunnerHandle::Get(),
                     std::move(collect_callback)));
}
