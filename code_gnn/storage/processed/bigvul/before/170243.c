  explicit MakeRequestFail(const std::string& host) : host_(host) {
    base::RunLoop run_loop;
    base::PostTaskWithTraitsAndReply(FROM_HERE, {BrowserThread::IO},
                                     base::BindOnce(MakeRequestFailOnIO, host_),
                                     run_loop.QuitClosure());
    run_loop.Run();
  }
