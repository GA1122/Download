void DataReductionProxyIOData::SetStringPref(const std::string& pref_path,
                                             const std::string& value) {
  DCHECK(io_task_runner_->BelongsToCurrentThread());
  ui_task_runner_->PostTask(
      FROM_HERE, base::BindOnce(&DataReductionProxyService::SetStringPref,
                                service_, pref_path, value));
}
