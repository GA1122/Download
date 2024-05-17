 virtual void DecompressedFrameHook(const vpx_image_t& img,
 const unsigned int frame_number) {
    ASSERT_TRUE(md5_file_ != NULL);
 char expected_md5[33];
 char junk[128];

 const int res = fscanf(md5_file_, "%s  %s", expected_md5, junk);
    ASSERT_NE(res, EOF) << "Read md5 data failed";
    expected_md5[32] = '\0';

 ::libvpx_test::MD5 md5_res;
    md5_res.Add(&img);
 const char *actual_md5 = md5_res.Get();

    ASSERT_STREQ(expected_md5, actual_md5)
 << "Md5 checksums don't match: frame number = " << frame_number;
 }
