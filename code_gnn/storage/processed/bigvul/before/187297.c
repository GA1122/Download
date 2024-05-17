 MediaGalleriesCustomBindings::MediaGalleriesCustomBindings(
      ScriptContext* context)
      : ObjectBackedNativeHandler(context) {
    RouteFunction(
      "GetMediaFileSystemObject",
//       "GetMediaFileSystemObject", "mediaGalleries",
        base::Bind(&MediaGalleriesCustomBindings::GetMediaFileSystemObject,
                   base::Unretained(this)));
  }