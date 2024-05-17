void PostClosure(MessageLoop* message_loop,
                 const tracked_objects::Location& from_here,
                 const base::Closure& callback) {
  message_loop->PostTask(from_here, callback);
}
