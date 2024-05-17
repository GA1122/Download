 void FillRandom() {
 ACMRandom rnd(ACMRandom::DeterministicSeed());
 for (int p = 0; p < num_planes_; p++) {
 for (int x = -1 ; x <= block_size_; x++)
        data_ptr_[p][x - stride_] = rnd.Rand8();
 for (int y = 0; y < block_size_; y++)
        data_ptr_[p][y * stride_ - 1] = rnd.Rand8();
 }
 }
