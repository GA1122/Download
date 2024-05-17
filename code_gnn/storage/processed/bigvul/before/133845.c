 bool Send(IPC::Message* message) {
  return content::UtilityThread::Get()->Send(message);
}
