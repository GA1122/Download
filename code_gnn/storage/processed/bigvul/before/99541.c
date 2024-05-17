 void PluginTest::registerCreateTestFunction(const string& identifier, CreateTestFunction createTestFunction)
 {
     assert(!createTestFunctions().count(identifier));
 
    createTestFunctions()[identifier] = createTestFunction;
}
