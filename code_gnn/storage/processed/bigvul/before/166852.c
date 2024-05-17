  ~ExpectWriteBarrierFires() {
    EXPECT_FALSE(marking_worklist_->IsGlobalEmpty());
    MarkingItem item;
    while (marking_worklist_->Pop(WorklistTaskId::MainThread, &item)) {
      if (!ThreadHeap::IsNormalArenaIndex(
              PageFromObject(item.object)->Arena()->ArenaIndex())) {
        backing_visitor_.ProcessBackingStore(
            HeapObjectHeader::FromPayload(item.object));
        continue;
      }
      auto pos = std::find(objects_.begin(), objects_.end(), item.object);
      if (objects_.end() != pos)
        objects_.erase(pos);
    }
    EXPECT_TRUE(objects_.empty());
    for (HeapObjectHeader* header : headers_) {
      EXPECT_TRUE(header->IsMarked());
      header->Unmark();
    }
    EXPECT_TRUE(marking_worklist_->IsGlobalEmpty());
  }
