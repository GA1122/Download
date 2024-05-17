 bool CreateBufferList(int num_buffers) {
 if (num_buffers < 0)
 return false;

    num_buffers_ = num_buffers;
    ext_fb_list_ = new ExternalFrameBuffer[num_buffers_];
    EXPECT_TRUE(ext_fb_list_ != NULL);
    memset(ext_fb_list_, 0, sizeof(ext_fb_list_[0]) * num_buffers_);
 return true;
 }
