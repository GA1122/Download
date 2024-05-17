 void CheckVPrediction() const {
 for (int p = 0; p < num_planes_; p++)
 for (int y = 0; y < block_size_; y++)
        ASSERT_EQ(0, memcmp(&data_ptr_[p][-stride_],
 &data_ptr_[p][y * stride_], block_size_));
 }
