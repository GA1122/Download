void RunTaskOnThread(scoped_refptr<base::MessageLoopProxy> relay_proxy,
                     const base::Closure& task) {
  if (relay_proxy->BelongsToCurrentThread()) {
    task.Run();
  } else {
    const bool posted = relay_proxy->PostTask(FROM_HERE, task);
    DCHECK(posted);
  }
}
