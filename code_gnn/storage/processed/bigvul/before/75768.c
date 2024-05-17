	void testUriHostEmpty() {
		UriParserStateA stateA;
		UriUriA uriA;
		stateA.uri = &uriA;
		const char * const input = "http" "://" ":" "123";
		const int res = uriParseUriA(&stateA, input);
		TEST_ASSERT(URI_SUCCESS == res);
		TEST_ASSERT(uriA.userInfo.first == NULL);
		TEST_ASSERT(uriA.userInfo.afterLast == NULL);
		TEST_ASSERT(uriA.hostText.first != NULL);
		TEST_ASSERT(uriA.hostText.afterLast != NULL);
		TEST_ASSERT(uriA.hostText.afterLast - uriA.hostText.first == 0);
		TEST_ASSERT(uriA.portText.first == input + 4 + 3 + 1);
		TEST_ASSERT(uriA.portText.afterLast == input + 4 + 3 + 1 + 3);
		uriFreeUriMembersA(&uriA);
	}
