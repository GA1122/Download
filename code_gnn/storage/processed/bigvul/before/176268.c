 static PropertyDetails GetDetailsImpl(FixedArrayBase* backing_store,
 uint32_t entry) {
 return PropertyDetails(kData, DONT_DELETE, 0, PropertyCellType::kNoCell);
 }
