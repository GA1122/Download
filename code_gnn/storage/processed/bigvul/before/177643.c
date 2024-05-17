 void SetLeftUnavailable() {
    mbptr_->left_available = 0;
 for (int p = 0; p < num_planes_; p++)
 for (int i = -1; i < block_size_; ++i)
        data_ptr_[p][stride_ * i - 1] = 129;
 }
