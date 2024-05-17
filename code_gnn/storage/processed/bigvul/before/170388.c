MPEG4Extractor::~MPEG4Extractor() {
 Track *track = mFirstTrack;
 while (track) {
 Track *next = track->next;

 delete track;
        track = next;
 }
    mFirstTrack = mLastTrack = NULL;

    SINF *sinf = mFirstSINF;
 while (sinf) {
        SINF *next = sinf->next;
 delete[] sinf->IPMPData;
 delete sinf;
        sinf = next;
 }
    mFirstSINF = NULL;

 for (size_t i = 0; i < mPssh.size(); i++) {
 delete [] mPssh[i].data;
 }
}
