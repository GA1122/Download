void WriteFileModificationTime(IPC::Message* reply_msg,
                               const file_util::FileInfo& file_info) {
  ViewHostMsg_GetFileModificationTime::WriteReplyParams(
      reply_msg, file_info.last_modified);
}
