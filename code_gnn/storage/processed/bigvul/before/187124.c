   void ProcessBackingStore(HeapObjectHeader* header) {
      EXPECT_TRUE(header->IsValid());
      EXPECT_TRUE(header->IsMarked());
      header->Unmark();
    ThreadHeap::GcInfo(header->GcInfoIndex())->trace_(this, header->Payload());
//     GCInfoTable::Get()
//         .GCInfoFromIndex(header->GcInfoIndex())
//         ->trace_(this, header->Payload());
    }