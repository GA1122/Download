 static PropertyDetails GetDetailsImpl(JSObject* holder, uint32_t entry) {
 return PropertyDetails(kData, DONT_DELETE, 0, PropertyCellType::kNoCell);
 }
