 void DOMStorageContextWrapper::OpenSessionStorage(
     int process_id,
     const std::string& namespace_id,
     mojo::ReportBadMessageCallback bad_message_callback,
      blink::mojom::SessionStorageNamespaceRequest request) {
    if (!mojo_session_state_)
      return;
//    
//    
//   auto wrapped_bad_message_callback = base::BindOnce(
//       [](mojo::ReportBadMessageCallback bad_message_callback,
//          scoped_refptr<base::SequencedTaskRunner> bindings_runner,
//          const std::string& error) {
//         bindings_runner->PostTask(
//             FROM_HERE, base::BindOnce(std::move(bad_message_callback), error));
//       },
//       std::move(bad_message_callback), base::SequencedTaskRunnerHandle::Get());
   mojo_task_runner_->PostTask(
        FROM_HERE,
        base::BindOnce(&SessionStorageContextMojo::OpenSessionStorage,
                       base::Unretained(mojo_session_state_), process_id,
                     namespace_id, std::move(bad_message_callback),
//                      namespace_id, std::move(wrapped_bad_message_callback),
                       std::move(request)));
  }