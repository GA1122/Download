bool Extension::HasCachedImage(const ExtensionResource& source,
                               const gfx::Size& max_size) const {
  DCHECK(source.extension_root() == path());   
  return GetCachedImageImpl(source, max_size) != NULL;
}
