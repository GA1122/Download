  void SaveResultAndQuit(bool success,
                         const history::URLRow&,
                         const history::VisitVector&) {
    query_url_success_ = success;
    quit_closure_.Run();
  }
