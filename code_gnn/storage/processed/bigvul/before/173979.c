long long Cluster::GetTimeCode() const {
 long long pos;
 long len;

 const long status = Load(pos, len);

 if (status < 0)  
 return status;

 return m_timecode;
}