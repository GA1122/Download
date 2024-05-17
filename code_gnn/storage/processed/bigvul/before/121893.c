  base::Time FakeNow() const {
    return now_ + now_delta_;
  }
