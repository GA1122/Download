PluginTest* PluginTest::create(NPP npp, const string& identifier)
{
    CreateTestFunction createTestFunction = createTestFunctions()[identifier];
    if (createTestFunction)
        return createTestFunction(npp, identifier);

    return new PluginTest(npp, identifier);
}
