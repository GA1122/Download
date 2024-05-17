ServiceManagerConnection* ServiceManagerConnection::GetForProcess() {
  return g_connection_for_process.Get().get();
}
