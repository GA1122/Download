    virtual void SetUp()
    {
        KURL secureURL(ParsedURLString, "https://example.test/image.png");
        RefPtr<SecurityOrigin> secureOrigin(SecurityOrigin::create(secureURL));
        document = Document::create();
        document->setSecurityOrigin(secureOrigin);
        csp->bindToExecutionContext(document.get());
    }
