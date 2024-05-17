void RenderViewImpl::deleteFileSystem(
    WebFrame* frame,
    WebFileSystem::Type type ,
    WebFileSystemCallbacks* callbacks) {
  DCHECK(callbacks);

  WebSecurityOrigin origin = frame->document().securityOrigin();
  if (origin.isUnique()) {
    callbacks->didSucceed();
    return;
  }

  ChildThread::current()->file_system_dispatcher()->DeleteFileSystem(
      GURL(origin.toString()),
      static_cast<fileapi::FileSystemType>(type),
      new WebFileSystemCallbackDispatcher(callbacks));
}
