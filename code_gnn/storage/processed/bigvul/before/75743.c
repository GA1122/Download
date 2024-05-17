	void testNormalizeSyntax() {
		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"eXAMPLE://a/./b/../b/%63/%7bfoo%7d",
				L"example://a/b/c/%7Bfoo%7D"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"http://examp%4Ce.com/",
				L"http://example.com/"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"http://example.com/a/b/%2E%2E/",
				L"http://example.com/a/"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"http://user:pass@SOMEHOST.COM:123",
				L"http://user:pass@somehost.com:123"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"HTTP://a:b@HOST:123/./1/2/../%41?abc#def",
				L"http://a:b@host:123/1/A?abc#def"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"../../abc",
				L"../../abc"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"../../abc/..",
				L"../../"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"../../abc/../def",
				L"../../def"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"abc/..",
				L""));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"abc/../",
				L""));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"../../abc/./def",
				L"../../abc/def"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"./def",
				L"def"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"def/.",
				L"def/"));

		TEST_ASSERT(testNormalizeSyntaxHelper(
				L"./abc:def",
				L"./abc:def"));
	}
