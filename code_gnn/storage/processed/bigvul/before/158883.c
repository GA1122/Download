  bool GetResult() const {
    bool res;
    CHECK(js_result_);
    CHECK(js_result_->GetAsBoolean(&res));
    return res;
  }
