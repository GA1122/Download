void ID3::Iterator::next() {
 if (mFrameData == NULL) {
 return;
 }

    mOffset += mFrameSize;

    findFrame();
}
