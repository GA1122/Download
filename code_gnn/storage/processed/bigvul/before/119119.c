void SetRTL(bool rtl) {
  SetICUDefaultLocale(rtl ? "he" : "en");
  EXPECT_EQ(rtl, IsRTL());
}
