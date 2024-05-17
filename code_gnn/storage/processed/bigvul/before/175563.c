size_t HevcParameterSets::getNumNalUnitsOfType(uint8_t type) {
 size_t num = 0;
 for (size_t i = 0; i < mNalUnits.size(); ++i) {
 if (getType(i) == type) {
 ++num;
 }
 }
 return num;
}
