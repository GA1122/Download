  void Wait(net::LoadState load_state, const base::string16& host) {
    waiting_host_ = host;
    waiting_state_ = load_state;
    if (!LoadStateMatches(web_contents_)) {
      base::RunLoop run_loop;
      quit_closure_ = run_loop.QuitClosure();
      run_loop.Run();
      DCHECK(LoadStateMatches(web_contents_));
    }
  }
