  void TearDownOnIO() {
    std::vector<Profile*> profiles =
        browser_process_->profile_manager()->GetLoadedProfiles();
    for (size_t i = 0; i < profiles.size(); ++i) {
      chrome_browser_net::Predictor* predictor =
          profiles[i]->GetNetworkPredictor();
      if (predictor) {
        predictor->EnablePredictorOnIOThread(false);
        predictor->Shutdown();
      }
    }
  }
