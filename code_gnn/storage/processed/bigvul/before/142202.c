  explicit MockFileTasksObserver(Profile* profile) : observer_(this) {
    observer_.Add(file_tasks::FileTasksNotifier::GetForProfile(profile));
  }
