ID3::Iterator::Iterator(const ID3 &parent, const char *id)
 : mParent(parent),
      mID(NULL),
      mOffset(mParent.mFirstFrameOffset),
      mFrameData(NULL),
      mFrameSize(0) {
 if (id) {
        mID = strdup(id);
 }

    findFrame();
}
