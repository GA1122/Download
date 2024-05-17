  void DoTruncationTest(const vector<int64>& unsynced_handle_view,
                        const vector<syncable::Id>& expected_id_order) {
    for (size_t limit = expected_id_order.size() + 2; limit > 0; --limit) {
      WriteTransaction wtrans(FROM_HERE, UNITTEST, directory());
      ScopedSetSessionWriteTransaction set_trans(session_.get(), &wtrans);

      ModelSafeRoutingInfo routes;
      GetModelSafeRoutingInfo(&routes);
      sessions::OrderedCommitSet output_set(routes);
      GetCommitIdsCommand command(limit, &output_set);
      std::set<int64> ready_unsynced_set;
      command.FilterUnreadyEntries(&wtrans, syncable::ModelTypeSet(),
                                   syncable::ModelTypeSet(), false,
                                   unsynced_handle_view, &ready_unsynced_set);
      command.BuildCommitIds(session_->write_transaction(), routes,
                             ready_unsynced_set);
      size_t truncated_size = std::min(limit, expected_id_order.size());
      ASSERT_EQ(truncated_size, output_set.Size());
      for (size_t i = 0; i < truncated_size; ++i) {
        ASSERT_EQ(expected_id_order[i], output_set.GetCommitIdAt(i))
            << "At index " << i << " with batch size limited to " << limit;
      }
      sessions::OrderedCommitSet::Projection proj;
      proj = output_set.GetCommitIdProjection(GROUP_PASSIVE);
      ASSERT_EQ(truncated_size, proj.size());
      for (size_t i = 0; i < truncated_size; ++i) {
        SCOPED_TRACE(::testing::Message("Projection mismatch with i = ") << i);
        syncable::Id projected = output_set.GetCommitIdAt(proj[i]);
        ASSERT_EQ(expected_id_order[proj[i]], projected);
        ASSERT_EQ(expected_id_order[i], projected);
      }
    }
  }
