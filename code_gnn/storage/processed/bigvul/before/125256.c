bool DatabaseMessageFilter::OnMessageReceived(
    const IPC::Message& message,
    bool* message_was_ok) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP_EX(DatabaseMessageFilter, message, *message_was_ok)
    IPC_MESSAGE_HANDLER_DELAY_REPLY(DatabaseHostMsg_OpenFile,
                                    OnDatabaseOpenFile)
    IPC_MESSAGE_HANDLER_DELAY_REPLY(DatabaseHostMsg_DeleteFile,
                                    OnDatabaseDeleteFile)
    IPC_MESSAGE_HANDLER_DELAY_REPLY(DatabaseHostMsg_GetFileAttributes,
                                    OnDatabaseGetFileAttributes)
    IPC_MESSAGE_HANDLER_DELAY_REPLY(DatabaseHostMsg_GetFileSize,
                                    OnDatabaseGetFileSize)
    IPC_MESSAGE_HANDLER_DELAY_REPLY(DatabaseHostMsg_GetSpaceAvailable,
                                    OnDatabaseGetSpaceAvailable)
    IPC_MESSAGE_HANDLER(DatabaseHostMsg_Opened, OnDatabaseOpened)
    IPC_MESSAGE_HANDLER(DatabaseHostMsg_Modified, OnDatabaseModified)
    IPC_MESSAGE_HANDLER(DatabaseHostMsg_Closed, OnDatabaseClosed)
    IPC_MESSAGE_HANDLER(DatabaseHostMsg_HandleSqliteError, OnHandleSqliteError)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP_EX()
  return handled;
}
