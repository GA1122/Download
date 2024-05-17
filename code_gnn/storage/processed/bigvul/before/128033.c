void AwContents::PostMessageToFrame(JNIEnv* env, jobject obj,
    jstring frame_name, jstring message, jstring target_origin,
    jintArray sent_ports) {

  base::string16 source_origin;
  base::string16 j_target_origin(ConvertJavaStringToUTF16(env, target_origin));
  base::string16 j_message(ConvertJavaStringToUTF16(env, message));
  std::vector<int> j_ports;

  if (sent_ports != nullptr) {
    base::android::JavaIntArrayToIntVector(env, sent_ports, &j_ports);
    BrowserThread::PostTask(
        BrowserThread::IO,
        FROM_HERE,
        base::Bind(&AwMessagePortServiceImpl::RemoveSentPorts,
                   base::Unretained(AwMessagePortServiceImpl::GetInstance()),
                   j_ports));
  }
  std::vector<content::TransferredMessagePort> ports(j_ports.size());
  for (size_t i = 0; i < j_ports.size(); ++i)
    ports[i].id = j_ports[i];
  content::MessagePortProvider::PostMessageToFrame(web_contents_.get(),
                                                   source_origin,
                                                   j_target_origin,
                                                   j_message,
                                                   ports);
}
