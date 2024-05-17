 virtual void EndPassHook(void) {
 for (int layer = 0; layer < static_cast<int>(cfg_.ts_number_layers);
 ++layer) {
      duration_ = (last_pts_ + 1) * timebase_;
 if (bits_total_[layer]) {
        effective_datarate_[layer] = (bits_total_[layer] / 1000.0) / duration_;
 }
 }
 }
