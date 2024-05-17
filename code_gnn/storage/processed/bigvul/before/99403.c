  void HandleGetChildWithInode(int fd, const Pickle& pickle, void* iter,
                               std::vector<int>& fds) {
    if (sandbox_cmd_.empty()) {
      LOG(ERROR) << "Not in the sandbox, this should not be called";
      return;
    }

    uint64_t inode;
    if (!pickle.ReadUInt64(&iter, &inode))
      return;

    base::ProcessId pid = 0;
    std::string inode_output;

    std::vector<std::string> sandbox_cmd = sandbox_cmd_;
    sandbox_cmd.push_back(Int64ToString(inode));
    CommandLine get_inode_cmd(sandbox_cmd);
    if (base::GetAppOutput(get_inode_cmd, &inode_output))
      StringToInt(inode_output, &pid);

    if (!pid) {
      LOG(ERROR) << "Could not get pid";
      return;
    }

    Pickle reply;
    reply.WriteInt(pid);
    SendRendererReply(fds, reply, -1);
  }
