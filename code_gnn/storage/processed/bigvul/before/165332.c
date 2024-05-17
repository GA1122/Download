  DOMStorageContextImplTest()
      : kOrigin(url::Origin::Create(GURL("http://dom_storage/"))),
        kKey(ASCIIToUTF16("key")),
        kValue(ASCIIToUTF16("value")),
        kDontIncludeFileInfo(false),
        kDoIncludeFileInfo(true) {}
