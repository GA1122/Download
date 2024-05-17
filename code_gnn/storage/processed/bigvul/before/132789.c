  PendingPacket(scoped_ptr<VideoPacket> packet, const base::Closure& done)
      : packet_(packet.Pass()),
        done_runner_(done) {
  }
