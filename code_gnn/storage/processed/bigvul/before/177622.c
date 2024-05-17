 virtual ~ExternalFrameBufferList() {
 for (int i = 0; i < num_buffers_; ++i) {
 delete [] ext_fb_list_[i].data;
 }
 delete [] ext_fb_list_;
 }
