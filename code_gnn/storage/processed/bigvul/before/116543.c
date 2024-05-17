  virtual ~TestSiteInstance() {
    (*delete_counter_)++;
  }
