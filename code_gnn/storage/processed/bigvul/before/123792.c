DOMFileSystemSync::DOMFileSystemSync(ExecutionContext* context, const String& name, FileSystemType type, const KURL& rootURL)
    : DOMFileSystemBase(context, name, type, rootURL)
{
    ScriptWrappable::init(this);
}
