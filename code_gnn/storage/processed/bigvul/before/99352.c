void WriteFileSize(IPC::Message* reply_msg,
                   const file_util::FileInfo& file_info) {
  ViewHostMsg_GetFileSize::WriteReplyParams(reply_msg, file_info.size);
}
