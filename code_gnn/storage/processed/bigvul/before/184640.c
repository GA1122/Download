  void ClientSession::Disconnect() {
  connection_->Disconnect();
//   DCHECK(connection_);
    authenticated_ = false;
    RestoreEventState();
// 
//    
//    
//   connection_->Disconnect();
  }