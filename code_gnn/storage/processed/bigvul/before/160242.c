  static void ManifestCallbackAndRun(const base::Closure& continuation,
                                     const GURL&,
                                     const content::Manifest&) {
    continuation.Run();
  }
