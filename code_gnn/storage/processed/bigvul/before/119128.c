void TokenizeTest() {
  std::vector<STR> r;
  size_t size;

  size = Tokenize(STR("This is a string"), STR(" "), &r);
  EXPECT_EQ(4U, size);
  ASSERT_EQ(4U, r.size());
  EXPECT_EQ(r[0], STR("This"));
  EXPECT_EQ(r[1], STR("is"));
  EXPECT_EQ(r[2], STR("a"));
  EXPECT_EQ(r[3], STR("string"));
  r.clear();

  size = Tokenize(STR("one,two,three"), STR(","), &r);
  EXPECT_EQ(3U, size);
  ASSERT_EQ(3U, r.size());
  EXPECT_EQ(r[0], STR("one"));
  EXPECT_EQ(r[1], STR("two"));
  EXPECT_EQ(r[2], STR("three"));
  r.clear();

  size = Tokenize(STR("one,two:three;four"), STR(",:"), &r);
  EXPECT_EQ(3U, size);
  ASSERT_EQ(3U, r.size());
  EXPECT_EQ(r[0], STR("one"));
  EXPECT_EQ(r[1], STR("two"));
  EXPECT_EQ(r[2], STR("three;four"));
  r.clear();

  size = Tokenize(STR("one,two:three;four"), STR(";,:"), &r);
  EXPECT_EQ(4U, size);
  ASSERT_EQ(4U, r.size());
  EXPECT_EQ(r[0], STR("one"));
  EXPECT_EQ(r[1], STR("two"));
  EXPECT_EQ(r[2], STR("three"));
  EXPECT_EQ(r[3], STR("four"));
  r.clear();

  size = Tokenize(STR("one, two, three"), STR(","), &r);
  EXPECT_EQ(3U, size);
  ASSERT_EQ(3U, r.size());
  EXPECT_EQ(r[0], STR("one"));
  EXPECT_EQ(r[1], STR(" two"));
  EXPECT_EQ(r[2], STR(" three"));
  r.clear();

  size = Tokenize(STR("one, two, three, "), STR(","), &r);
  EXPECT_EQ(4U, size);
  ASSERT_EQ(4U, r.size());
  EXPECT_EQ(r[0], STR("one"));
  EXPECT_EQ(r[1], STR(" two"));
  EXPECT_EQ(r[2], STR(" three"));
  EXPECT_EQ(r[3], STR(" "));
  r.clear();

  size = Tokenize(STR("one, two, three,"), STR(","), &r);
  EXPECT_EQ(3U, size);
  ASSERT_EQ(3U, r.size());
  EXPECT_EQ(r[0], STR("one"));
  EXPECT_EQ(r[1], STR(" two"));
  EXPECT_EQ(r[2], STR(" three"));
  r.clear();

  size = Tokenize(STR(), STR(","), &r);
  EXPECT_EQ(0U, size);
  ASSERT_EQ(0U, r.size());
  r.clear();

  size = Tokenize(STR(","), STR(","), &r);
  EXPECT_EQ(0U, size);
  ASSERT_EQ(0U, r.size());
  r.clear();

  size = Tokenize(STR(",;:."), STR(".:;,"), &r);
  EXPECT_EQ(0U, size);
  ASSERT_EQ(0U, r.size());
  r.clear();

  size = Tokenize(STR("\t\ta\t"), STR("\t"), &r);
  EXPECT_EQ(1U, size);
  ASSERT_EQ(1U, r.size());
  EXPECT_EQ(r[0], STR("a"));
  r.clear();

  size = Tokenize(STR("\ta\t\nb\tcc"), STR("\n"), &r);
  EXPECT_EQ(2U, size);
  ASSERT_EQ(2U, r.size());
  EXPECT_EQ(r[0], STR("\ta\t"));
  EXPECT_EQ(r[1], STR("b\tcc"));
  r.clear();
}
