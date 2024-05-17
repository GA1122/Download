static void PostCopyCallbackToMainThread(
    scoped_refptr<base::SingleThreadTaskRunner> main_thread_task_runner,
    scoped_ptr<CopyOutputRequest> request,
    scoped_ptr<CopyOutputResult> result) {
  main_thread_task_runner->PostTask(FROM_HERE,
                                    base::Bind(&RunCopyCallbackOnMainThread,
                                               base::Passed(&request),
                                               base::Passed(&result)));
}
