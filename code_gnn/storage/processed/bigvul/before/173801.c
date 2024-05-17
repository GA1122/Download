SoftVorbis::~SoftVorbis() {
 if (mState != NULL) {
        vorbis_dsp_clear(mState);
 delete mState;
        mState = NULL;
 }

 if (mVi != NULL) {
        vorbis_info_clear(mVi);
 delete mVi;
        mVi = NULL;
 }
}
