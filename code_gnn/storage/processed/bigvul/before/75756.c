	void testRangeComparison_RemoveBaseUri_Issue19() {
		testRemoveBaseUriHelper("scheme://host/source",
								"scheme://host/source",
								"schemelonger://host/base");
		testRemoveBaseUriHelper("schemelonger://host/source",
								"schemelonger://host/source",
								"scheme://host/base");

		testRemoveBaseUriHelper("//host/source",
								"http://host/source",
								"http://hostlonger/base");
		testRemoveBaseUriHelper("//hostlonger/source",
								"http://hostlonger/source",
								"http://host/base");

		testRemoveBaseUriHelper("//[v7.host]/source",
								"http://[v7.host]/source",
								"http://[v7.hostlonger]/base");
		testRemoveBaseUriHelper("//[v7.hostlonger]/source",
								"http://[v7.hostlonger]/source",
								"http://host/base");

		testRemoveBaseUriHelper("path1",
								"http://host/path1",
								"http://host/path111");
		testRemoveBaseUriHelper("../path1/path2",
								"http://host/path1/path2",
								"http://host/path111/path222");
		testRemoveBaseUriHelper("path111",
								"http://host/path111",
								"http://host/path1");
		testRemoveBaseUriHelper("../path111/path222",
								"http://host/path111/path222",
								"http://host/path1/path2");

		testRemoveBaseUriHelper("//example/x/abc",
								"http://example/x/abc",
								"http://example2/x/y/z");
	}
