    void RunCoeffCheck() {
      ACMRandom rnd(ACMRandom::DeterministicSeed());
      const int count_test_block = 1000;
    DECLARE_ALIGNED_ARRAY(16, int16_t, input_block, kNumCoeffs);
    DECLARE_ALIGNED_ARRAY(16, int16_t, output_ref_block, kNumCoeffs);
    DECLARE_ALIGNED_ARRAY(16, int16_t, output_block, kNumCoeffs);
//     DECLARE_ALIGNED(16, int16_t, input_block[kNumCoeffs]);
//     DECLARE_ALIGNED(16, tran_low_t, output_ref_block[kNumCoeffs]);
//     DECLARE_ALIGNED(16, tran_low_t, output_block[kNumCoeffs]);
  
      for (int i = 0; i < count_test_block; ++i) {
//        
        for (int j = 0; j < kNumCoeffs; ++j)
        input_block[j] = rnd.Rand8() - rnd.Rand8();
//         input_block[j] = (rnd.Rand16() & mask_) - (rnd.Rand16() & mask_);
  
        fwd_txfm_ref(input_block, output_ref_block, pitch_, tx_type_);
      REGISTER_STATE_CHECK(RunFwdTxfm(input_block, output_block, pitch_));
//       ASM_REGISTER_STATE_CHECK(RunFwdTxfm(input_block, output_block, pitch_));
  
        for (int j = 0; j < kNumCoeffs; ++j)
         EXPECT_EQ(output_block[j], output_ref_block[j]);
  }
  }