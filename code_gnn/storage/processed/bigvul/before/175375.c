MyOggExtractor::~MyOggExtractor() {
    vorbis_comment_clear(&mVc);
    vorbis_info_clear(&mVi);
}
