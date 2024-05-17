 void SetTopUnavailable() {
    mbptr_->up_available = 0;
 for (int p = 0; p < num_planes_; p++)
      memset(&data_ptr_[p][-1 - stride_], 127, block_size_ + 2);
 }
