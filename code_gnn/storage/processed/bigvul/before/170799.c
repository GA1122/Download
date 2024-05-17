static jboolean Bitmap_compress(JNIEnv* env, jobject clazz, jlong bitmapHandle,
                                jint format, jint quality,
                                jobject jstream, jbyteArray jstorage) {
 SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
 SkImageEncoder::Type fm;

 switch (format) {
 case kJPEG_JavaEncodeFormat:
        fm = SkImageEncoder::kJPEG_Type;
 break;
 case kPNG_JavaEncodeFormat:
        fm = SkImageEncoder::kPNG_Type;
 break;
 case kWEBP_JavaEncodeFormat:
        fm = SkImageEncoder::kWEBP_Type;
 break;
 default:
 return JNI_FALSE;
 }

 bool success = false;
 if (NULL != bitmap) {
 SkAutoLockPixels alp(*bitmap);

 if (NULL == bitmap->getPixels()) {
 return JNI_FALSE;
 }

 SkWStream* strm = CreateJavaOutputStreamAdaptor(env, jstream, jstorage);
 if (NULL == strm) {
 return JNI_FALSE;
 }

 SkImageEncoder* encoder = SkImageEncoder::Create(fm);
 if (NULL != encoder) {
            success = encoder->encodeStream(strm, *bitmap, quality);
 delete encoder;
 }
 delete strm;
 }
 return success ? JNI_TRUE : JNI_FALSE;
}
