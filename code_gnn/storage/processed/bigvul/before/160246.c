  void WriteFile(const base::FilePath::StringType& filename,
                 base::StringPiece contents) {
    base::ScopedAllowBlockingForTesting allow_blocking;
    EXPECT_EQ(base::checked_cast<int>(contents.size()),
              base::WriteFile(service_worker_dir_.GetPath().Append(filename),
                              contents.data(), contents.size()));
  }
