 int FindFreeBufferIndex() {
 int i;
 for (i = 0; i < num_buffers_; ++i) {
 if (!ext_fb_list_[i].in_use)
 break;
 }
 return i;
 }
