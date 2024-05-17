 const GDataFile* GDataEntry::AsGDataFileConst() const {
   return const_cast<GDataEntry*>(this)->AsGDataFile();
}
