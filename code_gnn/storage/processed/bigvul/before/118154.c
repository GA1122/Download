void WebContentsAndroid::ReleaseMediaPlayers(JNIEnv* env, jobject jobj) {
#if defined(ENABLE_BROWSER_CDMS)
  web_contents_->ForEachFrame(
      base::Bind(&ReleaseAllMediaPlayers, base::Unretained(web_contents_)));
#endif  
}
