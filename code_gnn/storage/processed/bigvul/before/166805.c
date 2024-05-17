  ExpectNoWriteBarrierFires(ThreadState* thread_state,
                            std::initializer_list<void*> objects)
      : IncrementalMarkingScope(thread_state) {
    EXPECT_TRUE(marking_worklist_->IsGlobalEmpty());
    for (void* object : objects_) {
      HeapObjectHeader* header = HeapObjectHeader::FromPayload(object);
      headers_.push_back({header, header->IsMarked()});
    }
  }
