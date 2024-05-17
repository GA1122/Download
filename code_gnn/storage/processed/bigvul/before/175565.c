uint8_t HevcParameterSets::getType(size_t index) {
    CHECK_LT(index, mNalUnits.size());
 return mNalUnits[index]->int32Data();
}
