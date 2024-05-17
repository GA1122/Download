bool ASessionDescription::getDurationUs(int64_t *durationUs) const {
 *durationUs = 0;

    CHECK(mIsValid);

 AString value;
 if (!findAttribute(0, "a=range", &value)) {
 return false;
 }

 if (strncmp(value.c_str(), "npt=", 4)) {
 return false;
 }

 float from, to;
 if (!parseNTPRange(value.c_str() + 4, &from, &to)) {
 return false;
 }

 *durationUs = (int64_t)((to - from) * 1E6);

 return true;
}
