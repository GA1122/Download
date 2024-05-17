  static v8::Handle<v8::Value> DecodeJPEG(const v8::Arguments& args) {
    static const char* kAllowedIds[] = {
        "haiffjcadagjlijoggckpgfnoeiflnem",
        "gnedhmakppccajfpfiihfcdlnpgomkcf",
        "fjcibdnjlbfnbfdjneajpipnlcppleek",
        "oflbaaikkabfdfkimeclgkackhdkpnip"   
    };
    static const std::vector<std::string> allowed_ids(
        kAllowedIds, kAllowedIds + arraysize(kAllowedIds));

    ExtensionImpl* v8_extension = GetFromArguments<ExtensionImpl>(args);
    const ::Extension* extension =
        v8_extension->GetExtensionForCurrentContext();
    if (!extension)
      return v8::Undefined();
    if (allowed_ids.end() == std::find(
        allowed_ids.begin(), allowed_ids.end(), extension->id())) {
      return v8::Undefined();
    }

    DCHECK(args.Length() == 1);
    DCHECK(args[0]->IsArray());
    v8::Local<v8::Object> jpeg_array = args[0]->ToObject();
    size_t jpeg_length =
        jpeg_array->Get(v8::String::New("length"))->Int32Value();

    std::string jpeg_array_string;
    jpeg_array_string.reserve(jpeg_length);

    for (size_t i = 0; i != jpeg_length; ++i) {
      jpeg_array_string.push_back(
          jpeg_array->Get(v8::Integer::New(i))->Int32Value());
    }

    SkBitmap bitmap;
    if (!webkit_glue::DecodeImage(jpeg_array_string, &bitmap))
      return v8::Undefined();
    if (bitmap.config() != SkBitmap::kARGB_8888_Config)
      return v8::Undefined();
    const int width = bitmap.width();
    const int height = bitmap.height();
    SkAutoLockPixels lockpixels(bitmap);
    const uint32_t* pixels = static_cast<uint32_t*>(bitmap.getPixels());
    if (!pixels)
      return v8::Undefined();

    v8::Local<v8::Object> bitmap_array = v8::Array::New(width * height);
    for (int i = 0; i != width * height; ++i) {
      bitmap_array->Set(v8::Integer::New(i),
                        v8::Integer::New(pixels[i] & 0xFFFFFF));
    }
    return bitmap_array;
  }
