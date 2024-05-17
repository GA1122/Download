bool WtsSessionProcessDelegate::Send(IPC::Message* message) {
   return core_->Send(message);
 }
