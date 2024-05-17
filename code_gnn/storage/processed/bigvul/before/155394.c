void InvokeCallbackOnThread(
    scoped_refptr<base::SequencedTaskRunner> task_runner,
    base::Callback<void(bool)> callback,
    bool result) {
  task_runner->PostTask(FROM_HERE, base::BindOnce(std::move(callback), result));
}
