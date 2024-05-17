  explicit RegisteringMixin(ObjectRegistry* registry) {
    HeapObjectHeader* header = GetHeapObjectHeader();
    const void* uninitialized_value = BlinkGC::kNotFullyConstructedObject;
    EXPECT_EQ(uninitialized_value, header);
    registry->insert(reinterpret_cast<void*>(this), this);
  }
