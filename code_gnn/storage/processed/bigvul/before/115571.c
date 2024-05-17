 void NetworkChangeNotifierLinux::Thread::OnFileCanReadWithoutBlocking(int fd) {
  DCHECK_EQ(fd, netlink_fd_);
  ListenForNotifications();
}
