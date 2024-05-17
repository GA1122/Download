T Parcel::readAligned() const {
    T result;
 if (readAligned(&result) != NO_ERROR) {
        result = 0;
 }

 return result;
}
