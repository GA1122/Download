    bool Unref() {
      if (--ref_ == 0) {
        delete this;
        return true;
      }
      return false;
    }
