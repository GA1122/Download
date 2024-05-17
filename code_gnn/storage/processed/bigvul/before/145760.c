  explicit CounterNatives(ScriptContext* context)
      : ObjectBackedNativeHandler(context), counter_(0) {
    RouteFunction("Get",
                  base::Bind(&CounterNatives::Get, base::Unretained(this)));
    RouteFunction(
        "Increment",
        base::Bind(&CounterNatives::Increment, base::Unretained(this)));
  }
