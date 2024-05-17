long Segment::Load() {
  assert(m_clusters == NULL);
  assert(m_clusterSize == 0);
  assert(m_clusterCount == 0);


 const long long header_status = ParseHeaders();

 if (header_status < 0)  
 return static_cast<long>(header_status);

 if (header_status > 0)  
 return E_BUFFER_NOT_FULL;

 if (m_pInfo == NULL || m_pTracks == NULL)
 return E_FILE_FORMAT_INVALID;

 for (;;) {
 const int status = LoadCluster();

 if (status < 0)  
 return status;

 if (status >= 1)  
 return 0;
 }
}