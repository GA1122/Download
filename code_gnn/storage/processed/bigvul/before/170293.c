  ~MakeRequestFail() {
    base::RunLoop run_loop;
    base::PostTaskWithTraitsAndReply(
        FROM_HERE, {BrowserThread::IO},
        base::BindOnce(UndoMakeRequestFailOnIO, host_), run_loop.QuitClosure());
    run_loop.Run();
  }
