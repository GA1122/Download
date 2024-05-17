  SyncerPositionTiebreakingTest()
      : low_id_(Id::CreateFromServerId("A")),
        mid_id_(Id::CreateFromServerId("M")),
        high_id_(Id::CreateFromServerId("Z")),
        next_revision_(1) {
    DCHECK(low_id_ < mid_id_);
    DCHECK(mid_id_ < high_id_);
    DCHECK(low_id_ < high_id_);
  }
