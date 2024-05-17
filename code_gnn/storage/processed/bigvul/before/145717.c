void ApiTestEnvironment::RunTest(const std::string& file_name,
                                 const std::string& test_name) {
  env()->RegisterTestFile("testBody", file_name);
  base::RunLoop run_loop;
  gin::ModuleRegistry::From(env()->context()->v8_context())->AddBuiltinModule(
      env()->isolate(),
      "testNatives",
      TestNatives::Create(env()->isolate(), run_loop.QuitClosure()).ToV8());
  base::MessageLoop::current()->PostTask(
      FROM_HERE,
      base::Bind(&ApiTestEnvironment::RunTestInner, base::Unretained(this),
                 test_name, run_loop.QuitClosure()));
  base::MessageLoop::current()->PostTask(
      FROM_HERE, base::Bind(&ApiTestEnvironment::RunPromisesAgain,
                            base::Unretained(this)));
  run_loop.Run();
}
