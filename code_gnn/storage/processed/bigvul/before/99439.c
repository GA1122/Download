bool Channel::ChannelImpl::ProcessIncomingMessages() {
  ssize_t bytes_read = 0;

  struct msghdr msg = {0};
  struct iovec iov = {input_buf_, Channel::kReadBufferSize};

  msg.msg_iovlen = 1;
  msg.msg_control = input_cmsg_buf_;

  for (;;) {
    msg.msg_iov = &iov;

    if (bytes_read == 0) {
      if (pipe_ == -1)
        return false;

#if !defined(OS_MACOSX)
      if (fd_pipe_ >= 0) {
        bytes_read = HANDLE_EINTR(read(pipe_, input_buf_,
                                       Channel::kReadBufferSize));
        msg.msg_controllen = 0;
      } else
#endif
      {
        msg.msg_controllen = sizeof(input_cmsg_buf_);
        bytes_read = HANDLE_EINTR(recvmsg(pipe_, &msg, MSG_DONTWAIT));
      }
      if (bytes_read < 0) {
        if (errno == EAGAIN) {
          return true;
#if defined(OS_MACOSX)
        } else if (errno == EPERM) {
          return false;
#endif   
        } else if (errno == ECONNRESET || errno == EPIPE) {
          return false;
        } else {
          PLOG(ERROR) << "pipe error (" << pipe_ << ")";
          return false;
        }
      } else if (bytes_read == 0) {
        return false;
      }
    }
    DCHECK(bytes_read);

    if (client_pipe_ != -1) {
      Singleton<PipeMap>()->RemoveAndClose(pipe_name_);
      client_pipe_ = -1;
    }

    const int* wire_fds = NULL;
    unsigned num_wire_fds = 0;


    if (msg.msg_controllen > 0) {
      for (struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg); cmsg;
           cmsg = CMSG_NXTHDR(&msg, cmsg)) {
        if (cmsg->cmsg_level == SOL_SOCKET &&
            cmsg->cmsg_type == SCM_RIGHTS) {
          const unsigned payload_len = cmsg->cmsg_len - CMSG_LEN(0);
          DCHECK(payload_len % sizeof(int) == 0);
          wire_fds = reinterpret_cast<int*>(CMSG_DATA(cmsg));
          num_wire_fds = payload_len / 4;

          if (msg.msg_flags & MSG_CTRUNC) {
            LOG(ERROR) << "SCM_RIGHTS message was truncated"
                       << " cmsg_len:" << cmsg->cmsg_len
                       << " fd:" << pipe_;
            for (unsigned i = 0; i < num_wire_fds; ++i)
              HANDLE_EINTR(close(wire_fds[i]));
            return false;
          }
          break;
        }
      }
    }

    const char *p;
    const char *end;
    if (input_overflow_buf_.empty()) {
      p = input_buf_;
      end = p + bytes_read;
    } else {
      if (input_overflow_buf_.size() >
         static_cast<size_t>(kMaximumMessageSize - bytes_read)) {
        input_overflow_buf_.clear();
        LOG(ERROR) << "IPC message is too big";
        return false;
      }
      input_overflow_buf_.append(input_buf_, bytes_read);
      p = input_overflow_buf_.data();
      end = p + input_overflow_buf_.size();
    }

    const int* fds = NULL;
    unsigned num_fds = 0;
    unsigned fds_i = 0;   

    if (input_overflow_fds_.empty()) {
      fds = wire_fds;
      num_fds = num_wire_fds;
    } else {
      if (num_wire_fds > 0) {
        const size_t prev_size = input_overflow_fds_.size();
        input_overflow_fds_.resize(prev_size + num_wire_fds);
        memcpy(&input_overflow_fds_[prev_size], wire_fds,
               num_wire_fds * sizeof(int));
      }
      fds = &input_overflow_fds_[0];
      num_fds = input_overflow_fds_.size();
    }

    while (p < end) {
      const char* message_tail = Message::FindNext(p, end);
      if (message_tail) {
        int len = static_cast<int>(message_tail - p);
        Message m(p, len);

        if (m.header()->num_fds) {
          const char* error = NULL;
          if (m.header()->num_fds > num_fds - fds_i) {
#if !defined(OS_MACOSX)
            if (!uses_fifo_) {
              char dummy;
              struct iovec fd_pipe_iov = { &dummy, 1 };
              msg.msg_iov = &fd_pipe_iov;
              msg.msg_controllen = sizeof(input_cmsg_buf_);
              ssize_t n = HANDLE_EINTR(recvmsg(fd_pipe_, &msg, MSG_DONTWAIT));
              if (n == 1 && msg.msg_controllen > 0) {
                for (struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg); cmsg;
                     cmsg = CMSG_NXTHDR(&msg, cmsg)) {
                  if (cmsg->cmsg_level == SOL_SOCKET &&
                      cmsg->cmsg_type == SCM_RIGHTS) {
                    const unsigned payload_len = cmsg->cmsg_len - CMSG_LEN(0);
                    DCHECK(payload_len % sizeof(int) == 0);
                    wire_fds = reinterpret_cast<int*>(CMSG_DATA(cmsg));
                    num_wire_fds = payload_len / 4;

                    if (msg.msg_flags & MSG_CTRUNC) {
                      LOG(ERROR) << "SCM_RIGHTS message was truncated"
                                 << " cmsg_len:" << cmsg->cmsg_len
                                 << " fd:" << pipe_;
                      for (unsigned i = 0; i < num_wire_fds; ++i)
                        HANDLE_EINTR(close(wire_fds[i]));
                      return false;
                    }
                    break;
                  }
                }
                if (input_overflow_fds_.empty()) {
                  fds = wire_fds;
                  num_fds = num_wire_fds;
                } else {
                  if (num_wire_fds > 0) {
                    const size_t prev_size = input_overflow_fds_.size();
                    input_overflow_fds_.resize(prev_size + num_wire_fds);
                    memcpy(&input_overflow_fds_[prev_size], wire_fds,
                           num_wire_fds * sizeof(int));
                  }
                  fds = &input_overflow_fds_[0];
                  num_fds = input_overflow_fds_.size();
                }
              }
            }
            if (m.header()->num_fds > num_fds - fds_i)
#endif
              error = "Message needs unreceived descriptors";
          }

          if (m.header()->num_fds >
              FileDescriptorSet::MAX_DESCRIPTORS_PER_MESSAGE) {
            error = "Message requires an excessive number of descriptors";
          }

          if (error) {
            LOG(WARNING) << error
                         << " channel:" << this
                         << " message-type:" << m.type()
                         << " header()->num_fds:" << m.header()->num_fds
                         << " num_fds:" << num_fds
                         << " fds_i:" << fds_i;
#if defined(CHROMIUM_SELINUX)
            LOG(WARNING) << "In the case of SELinux this can be caused when "
                            "using a --user-data-dir to which the default "
                            "policy doesn't give the renderer access to. ";
#endif
            for (unsigned i = fds_i; i < num_fds; ++i)
              HANDLE_EINTR(close(fds[i]));
            input_overflow_fds_.clear();
            return false;
          }

          m.file_descriptor_set()->SetDescriptors(
              &fds[fds_i], m.header()->num_fds);
          fds_i += m.header()->num_fds;
        }
#ifdef IPC_MESSAGE_DEBUG_EXTRA
        DLOG(INFO) << "received message on channel @" << this <<
                      " with type " << m.type();
#endif
        if (m.routing_id() == MSG_ROUTING_NONE &&
            m.type() == HELLO_MESSAGE_TYPE) {
          void *iter = NULL;
          int pid;
          if (!m.ReadInt(&iter, &pid)) {
            NOTREACHED();
          }
#if !defined(OS_MACOSX)
          if (mode_ == MODE_SERVER && !uses_fifo_) {
            DCHECK_EQ(m.file_descriptor_set()->size(), 1);
            base::FileDescriptor descriptor;
            if (!m.ReadFileDescriptor(&iter, &descriptor)) {
              NOTREACHED();
            }
            fd_pipe_ = descriptor.fd;
            CHECK(descriptor.auto_close);
          }
#endif
          listener_->OnChannelConnected(pid);
        } else {
          listener_->OnMessageReceived(m);
        }
        p = message_tail;
      } else {
        break;
      }
      input_overflow_fds_ = std::vector<int>(&fds[fds_i], &fds[num_fds]);
      fds_i = 0;
      fds = &input_overflow_fds_[0];
      num_fds = input_overflow_fds_.size();
    }
    input_overflow_buf_.assign(p, end - p);
    input_overflow_fds_ = std::vector<int>(&fds[fds_i], &fds[num_fds]);

    if (input_overflow_buf_.empty() && !input_overflow_fds_.empty()) {
      return false;
    }

    bytes_read = 0;   
  }

  return true;
}
