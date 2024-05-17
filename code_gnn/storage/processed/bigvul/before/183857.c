    virtual bool Speak(
//       int utterance_id,
        const std::string& utterance,
      const std::string& language,
      const std::string& gender,
      double rate,
      double pitch,
      double volume) {
//       const std::string& lang,
//       const UtteranceContinuousParameters& params) {
      error_ = kNotSupportedError;
      return false;
    }