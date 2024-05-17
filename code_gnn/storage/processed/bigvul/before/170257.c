    explicit QuitMessageLoopAfterScreenshot(base::OnceClosure done)
        : done_(std::move(done)) {}
