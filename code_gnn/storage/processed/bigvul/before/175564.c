size_t HevcParameterSets::getSize(size_t index) {
    CHECK_LT(index, mNalUnits.size());
 return mNalUnits[index]->size();
}
