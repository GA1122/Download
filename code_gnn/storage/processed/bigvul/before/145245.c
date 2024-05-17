void Dispatcher::OnTransferBlobs(const std::vector<std::string>& blob_uuids) {
  RenderThread::Get()->Send(new ExtensionHostMsg_TransferBlobsAck(blob_uuids));
}
