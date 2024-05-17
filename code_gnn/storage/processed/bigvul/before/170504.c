status_t Parcel::writeAligned(T val) {
    COMPILE_TIME_ASSERT_FUNCTION_SCOPE(PAD_SIZE(sizeof(T)) == sizeof(T));

 if ((mDataPos+sizeof(val)) <= mDataCapacity) {
restart_write:
 *reinterpret_cast<T*>(mData+mDataPos) = val;
 return finishWrite(sizeof(val));
 }

 status_t err = growData(sizeof(val));
 if (err == NO_ERROR) goto restart_write;
 return err;
}
