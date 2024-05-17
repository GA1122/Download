  ui::PageTransition transition() {
    EXPECT_EQ(1U, transitions_.size());
    return transitions_[0];
  }
