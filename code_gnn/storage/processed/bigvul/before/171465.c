bool ASessionDescription::findAttribute(
 size_t index, const char *key, AString *value) const {
    CHECK_GE(index, 0u);
    CHECK_LT(index, mTracks.size());

    value->clear();

 const Attribs &track = mTracks.itemAt(index);
 ssize_t i = track.indexOfKey(AString(key));

 if (i < 0) {
 return false;
 }

 *value = track.valueAt(i);

 return true;
}
