static int getPremulBitmapCreateFlags(bool isMutable) {
 int flags = GraphicsJNI::kBitmapCreateFlag_Premultiplied;
 if (isMutable) flags |= GraphicsJNI::kBitmapCreateFlag_Mutable;
 return flags;
}
