   void CheckGuardBlocks() {
 for (int i = 0; i < kOutputBufferSize; ++i) {
 if (IsIndexInBorder(i))
        EXPECT_EQ(255, output_[i]);

     }
   }
