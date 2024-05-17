  ExpectWriteBarrierFires(ThreadState* thread_state,
                          std::initializer_list<void*> objects)
      : IncrementalMarkingScope(thread_state),
        objects_(objects),
        backing_visitor_(thread_state_, &objects_) {
    EXPECT_TRUE(marking_worklist_->IsGlobalEmpty());
    for (void* object : objects_) {
      CHECK(ThreadHeap::IsNormalArenaIndex(
          PageFromObject(object)->Arena()->ArenaIndex()));
      headers_.push_back(HeapObjectHeader::FromPayload(object));
      EXPECT_FALSE(headers_.back()->IsMarked());
    }
    EXPECT_FALSE(objects_.empty());
  }
