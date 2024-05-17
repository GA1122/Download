  void AddLinks(const VisitedLinkCommon::Fingerprints& links) {
    if (reset_needed_)
      return;

    if (pending_.size() + links.size() > kVisitedLinkBufferThreshold) {
      AddReset();
      return;
    }

    pending_.insert(pending_.end(), links.begin(), links.end());
  }
