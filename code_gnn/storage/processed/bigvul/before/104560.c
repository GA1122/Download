  static void DoMoreThan2PerMinuteFor5Minutes(const TimeTicks& start_time,
                                              QuotaLimitHeuristic* lim,
                                              Bucket* b,
                                              int an_unexhausted_minute) {
    for (int i = 0; i < 5; i++) {
      int m = i * 60;
      EXPECT_TRUE(lim->Apply(b, start_time + TimeDelta::FromSeconds(10 + m)));
      EXPECT_TRUE(b->has_tokens());

      if (i == an_unexhausted_minute)
        continue;   

      EXPECT_TRUE(lim->Apply(b, start_time + TimeDelta::FromSeconds(15 + m)));
      EXPECT_FALSE(b->has_tokens());

      EXPECT_TRUE(lim->Apply(b, start_time + TimeDelta::FromSeconds(20 + m)));
      EXPECT_FALSE(b->has_tokens());
      EXPECT_TRUE(lim->Apply(b, start_time + TimeDelta::FromSeconds(50 + m)));
      EXPECT_FALSE(b->has_tokens());
    }
  }
