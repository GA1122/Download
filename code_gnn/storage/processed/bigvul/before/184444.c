   void CreateSymbolicLink(const FilePath& target, const FilePath& symlink) {
    ASSERT_TRUE(file_util::CreateSymbolicLink(target, symlink))
        << ": " << target.value() << ": " << symlink.value();
//   virtual void TearDown() OVERRIDE {
//     metadata_.reset();
    }