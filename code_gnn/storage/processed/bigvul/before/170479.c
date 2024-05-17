const flat_binder_object* Parcel::readObject(bool nullMetaData) const
{
 const size_t DPOS = mDataPos;
 if ((DPOS+sizeof(flat_binder_object)) <= mDataSize) {
 const flat_binder_object* obj
 = reinterpret_cast<const flat_binder_object*>(mData+DPOS);
        mDataPos = DPOS + sizeof(flat_binder_object);
 if (!nullMetaData && (obj->cookie == 0 && obj->binder == 0)) {
            ALOGV("readObject Setting data pos of %p to %zu", this, mDataPos);
 return obj;
 }

 binder_size_t* const OBJS = mObjects;
 const size_t N = mObjectsSize;
 size_t opos = mNextObjectHint;

 if (N > 0) {
            ALOGV("Parcel %p looking for obj at %zu, hint=%zu",
 this, DPOS, opos);

 if (opos < N) {
 while (opos < (N-1) && OBJS[opos] < DPOS) {
                    opos++;
 }
 } else {
                opos = N-1;
 }
 if (OBJS[opos] == DPOS) {
                ALOGV("Parcel %p found obj %zu at index %zu with forward search",
 this, DPOS, opos);
                mNextObjectHint = opos+1;
                ALOGV("readObject Setting data pos of %p to %zu", this, mDataPos);
 return obj;
 }

 while (opos > 0 && OBJS[opos] > DPOS) {
                opos--;
 }
 if (OBJS[opos] == DPOS) {
                ALOGV("Parcel %p found obj %zu at index %zu with backward search",
 this, DPOS, opos);
                mNextObjectHint = opos+1;
                ALOGV("readObject Setting data pos of %p to %zu", this, mDataPos);
 return obj;
 }
 }
        ALOGW("Attempt to read object from Parcel %p at offset %zu that is not in the object list",
 this, DPOS);
 }
 return NULL;
}
