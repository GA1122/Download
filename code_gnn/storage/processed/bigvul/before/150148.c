  TestSwapPromiseResult()
      : did_activate_called(false),
        did_swap_called(false),
        did_not_swap_called(false),
        dtor_called(false),
        reason(SwapPromise::COMMIT_FAILS) {}
