void SparseBitSet::clear() {
    mMaxVal = 0;
    mIndices.reset();
    mBitmaps.reset();
}
