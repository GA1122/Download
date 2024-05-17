void PrintJobWorker::GetSettingsDone(PrintingContext::Result result) {
  MessageLoop::current()->SetNestableTasksAllowed(false);


  owner_->message_loop()->PostTask(FROM_HERE, NewRunnableMethod(
      owner_,
      &PrintJobWorkerOwner::GetSettingsDone,
      printing_context_.settings(),
      result));
 }
