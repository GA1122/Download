 void CheckHPrediction() const {
 for (int p = 0; p < num_planes_; p++)
 for (int y = 0; y < block_size_; y++)
 for (int x = 0; x < block_size_; x++)
          ASSERT_EQ(data_ptr_[p][-1 + y * stride_],
                    data_ptr_[p][x + y * stride_]);
 }
