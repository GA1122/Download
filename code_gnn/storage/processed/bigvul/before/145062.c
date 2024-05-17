    static PassOwnPtrWillBeRawPtr<MockScriptLoader> create(Element* element)
    {
        return adoptPtrWillBeNoop(new MockScriptLoader(element));
    }
