  bool CheckStep(T next, T* var) {
    int expected = next - 1;
    EXPECT_EQ(expected, *var);
    bool correct = expected == *var;
    *var = next;
    return correct;
  }
