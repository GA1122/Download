	void testDistinction() {
		 
		TEST_ASSERT(testDistinctionHelper("s://", true, false, false));
		TEST_ASSERT(testDistinctionHelper("s:///", true, false, true));
		TEST_ASSERT(testDistinctionHelper("s://a", true, false, false));
		TEST_ASSERT(testDistinctionHelper("s://a/", true, false, true));
		TEST_ASSERT(testDistinctionHelper("s:/", false, true, false));
		TEST_ASSERT(testDistinctionHelper("s:a", false, false, false));
		TEST_ASSERT(testDistinctionHelper("s:a/", false, false, true));
		TEST_ASSERT(testDistinctionHelper("s:", false, false, false));

		TEST_ASSERT(testDistinctionHelper("//", true, false, false));
		TEST_ASSERT(testDistinctionHelper("///", true, false, true));
		TEST_ASSERT(testDistinctionHelper("/", false, true, false));
		TEST_ASSERT(testDistinctionHelper("a", false, false, false));
		TEST_ASSERT(testDistinctionHelper("a/", false, false, true));
		TEST_ASSERT(testDistinctionHelper("", false, false, false));
	}
