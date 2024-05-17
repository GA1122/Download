std::map<std::string, PluginTest::CreateTestFunction>& PluginTest::createTestFunctions()
{
    static std::map<std::string, CreateTestFunction> testFunctions;
    
    return testFunctions;
}
