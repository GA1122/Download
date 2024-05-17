void Parcel::scanForFds() const
{
 bool hasFds = false;
 for (size_t i=0; i<mObjectsSize; i++) {
 const flat_binder_object* flat
 = reinterpret_cast<const flat_binder_object*>(mData + mObjects[i]);
 if (flat->type == BINDER_TYPE_FD) {
            hasFds = true;
 break;
 }
 }
    mHasFds = hasFds;
    mFdsKnown = true;
}
