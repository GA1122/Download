  bool PrepareUsbTestEntries(Profile* profile) {
    if (!CreateRootDirectory(profile))
      return false;

    CreateEntry(AddEntriesMessage::TestEntryInfo(AddEntriesMessage::FILE,
                                                 "text.txt", "hello.txt")
                    .SetMimeType("text/plain"));
    CreateEntry(AddEntriesMessage::TestEntryInfo(AddEntriesMessage::DIRECTORY,
                                                 "", "Folder"));

    base::RunLoop().RunUntilIdle();
    return true;
  }
