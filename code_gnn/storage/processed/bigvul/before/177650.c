 static void TearDownTestCase() {
    vpx_free(data_array_);
    vpx_free(mi_);
    vpx_free(mb_);
    data_array_ = NULL;
 }
