bool HevcParameterSets::write(size_t index, uint8_t* dest, size_t size) {
    CHECK_LT(index, mNalUnits.size());
 const sp<ABuffer>& nalUnit = mNalUnits[index];
 if (size < nalUnit->size()) {
        ALOGE("dest buffer size too small: %zu vs. %zu to be written",
                size, nalUnit->size());
 return false;
 }
    memcpy(dest, nalUnit->data(), nalUnit->size());
 return true;
}
