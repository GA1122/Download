static void SetAwDrawSWFunctionTable(JNIEnv* env,
                                     const JavaParamRef<jclass>&,
                                     jlong function_table) {
  RasterHelperSetAwDrawSWFunctionTable(
      reinterpret_cast<AwDrawSWFunctionTable*>(function_table));
}
