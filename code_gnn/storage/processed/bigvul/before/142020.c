  TestForReentrancy()
      : cb_already_run(false),
        cb(BindRepeating(&TestForReentrancy::AssertCBIsNull,
                         Unretained(this))) {}
