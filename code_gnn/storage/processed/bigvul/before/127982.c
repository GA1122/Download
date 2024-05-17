void AwContents::CreateMessageChannel(JNIEnv* env, jobject obj,
    jobjectArray ports) {

  AwMessagePortServiceImpl::GetInstance()->CreateMessageChannel(env, ports,
      GetMessagePortMessageFilter());
}
