  void DoTruncationTest(const ScopedDirLookup& dir,
                        const vector<int64>& unsynced_handle_view,
                        const vector<syncable::Id>& expected_id_order) {
    for (size_t limit = expected_id_order.size() + 2; limit > 0; --limit) {
      StatusController* status = session_->status_controller();
      WriteTransaction wtrans(FROM_HERE, UNITTEST, dir);
      ScopedSetSessionWriteTransaction set_trans(session_.get(), &wtrans);
      status->set_unsynced_handles(unsynced_handle_view);

      ModelSafeRoutingInfo routes;
      GetModelSafeRoutingInfo(&routes);
      GetCommitIdsCommand command(limit);
      command.BuildCommitIds(session_->status_controller()->unsynced_handles(),
          session_->write_transaction(), routes);
      vector<syncable::Id> output =
          command.ordered_commit_set_->GetAllCommitIds();
      size_t truncated_size = std::min(limit, expected_id_order.size());
      ASSERT_TRUE(truncated_size == output.size());
      for (size_t i = 0; i < truncated_size; ++i) {
        ASSERT_TRUE(expected_id_order[i] == output[i])
            << "At index " << i << " with batch size limited to " << limit;
      }
      sessions::OrderedCommitSet::Projection proj;
      proj = command.ordered_commit_set_->GetCommitIdProjection(GROUP_PASSIVE);
      ASSERT_EQ(truncated_size, proj.size());
      for (size_t i = 0; i < truncated_size; ++i) {
        SCOPED_TRACE(::testing::Message("Projection mismatch with i = ") << i);
        syncable::Id projected =
            command.ordered_commit_set_->GetCommitIdAt(proj[i]);
        ASSERT_TRUE(expected_id_order[proj[i]] == projected);
        ASSERT_TRUE(expected_id_order[i] == projected);
      }
    }
  }
