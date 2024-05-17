  InitialLoadObserver(GDataFileSystemInterface* file_system,
                      const base::Closure& callback)
      : file_system_(file_system), callback_(callback) {}
