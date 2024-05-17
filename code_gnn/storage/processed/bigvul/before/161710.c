  ~InputBuffer() {
    VLOGF(4) << "id = " << id_;
    if (release_cb_)
      std::move(release_cb_).Run(id_);
  }
