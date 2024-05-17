ID3::Iterator::~Iterator() {
 if (mID) {
        free(mID);
        mID = NULL;
 }
}
