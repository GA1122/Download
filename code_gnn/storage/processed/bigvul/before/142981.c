  static WebMediaStubLocalFrameClient* Create(
      std::unique_ptr<WebMediaPlayer> player) {
    return MakeGarbageCollected<WebMediaStubLocalFrameClient>(
        std::move(player));
  }
