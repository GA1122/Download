 static void TearDownTestCase() {
    vpx_free(src_);
    src_ = NULL;
    vpx_free(dst_);
    dst_ = NULL;
    vpx_free(dst_c_);
    dst_c_ = NULL;
 }
