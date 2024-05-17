GetCertificateChain(JNIEnv* env, jobject obj, jobject java_web_contents) {
  content::WebContents* web_contents =
      content::WebContents::FromJavaWebContents(java_web_contents);
  if (!web_contents)
    return ScopedJavaLocalRef<jobjectArray>();

  int cert_id =
      web_contents->GetController().GetVisibleEntry()->GetSSL().cert_id;
  scoped_refptr<net::X509Certificate> cert;
  bool ok = CertStore::GetInstance()->RetrieveCert(cert_id, &cert);
  CHECK(ok);

  std::vector<std::string> cert_chain;
  net::X509Certificate::OSCertHandles cert_handles =
      cert->GetIntermediateCertificates();
  if (cert_handles.empty() || cert_handles[0] != cert->os_cert_handle())
    cert_handles.insert(cert_handles.begin(), cert->os_cert_handle());

  cert_chain.reserve(cert_handles.size());
  for (net::X509Certificate::OSCertHandles::const_iterator it =
           cert_handles.begin();
       it != cert_handles.end();
       ++it) {
    std::string cert_bytes;
    net::X509Certificate::GetDEREncoded(*it, &cert_bytes);
    cert_chain.push_back(cert_bytes);
  }

  return base::android::ToJavaArrayOfByteArray(env, cert_chain);
}
