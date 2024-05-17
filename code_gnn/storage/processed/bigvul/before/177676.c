 virtual void SetUp() {
    width_ = GET_PARAM(0);
    height_ = GET_PARAM(1);
    sixtap_predict_ = GET_PARAM(2);
    memset(src_, 0, kSrcSize);
    memset(dst_, 0, kDstSize);
    memset(dst_c_, 0, kDstSize);
 }
