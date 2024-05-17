bool CuePoint::Load(IMkvReader* pReader) {

 if (m_timecode >= 0)  
 return true;

  assert(m_track_positions == NULL);
  assert(m_track_positions_count == 0);

 long long pos_ = -m_timecode;
 const long long element_start = pos_;

 long long stop;

 {
 long len;

 const long long id = ReadID(pReader, pos_, len);
 if (id != 0x3B)
 return false;

    pos_ += len;  

 const long long size = ReadUInt(pReader, pos_, len);
    assert(size >= 0);

    pos_ += len;  

    stop = pos_ + size;
 }

 const long long element_size = stop - element_start;

 long long pos = pos_;


 while (pos < stop) {
 long len;

 const long long id = ReadID(pReader, pos, len);
 if ((id < 0) || (pos + len > stop)) {
 return false;
 }

    pos += len;  

 const long long size = ReadUInt(pReader, pos, len);
 if ((size < 0) || (pos + len > stop)) {
 return false;
 }

    pos += len;  
 if ((pos + size) > stop) {
 return false;
 }

 if (id == 0x33)  
      m_timecode = UnserializeUInt(pReader, pos, size);

 else if (id == 0x37)  
 ++m_track_positions_count;

    pos += size;  
 }

 if (m_timecode < 0 || m_track_positions_count <= 0) {
 return false;
 }


  m_track_positions = new (std::nothrow) TrackPosition[m_track_positions_count];
 if (m_track_positions == NULL)
 return false;


 TrackPosition* p = m_track_positions;
  pos = pos_;

 while (pos < stop) {
 long len;

 const long long id = ReadID(pReader, pos, len);
 if (id < 0 || (pos + len) > stop)
 return false;

    pos += len;  

 const long long size = ReadUInt(pReader, pos, len);
    assert(size >= 0);
    assert((pos + len) <= stop);

    pos += len;  
    assert((pos + size) <= stop);

 if (id == 0x37) {  
 TrackPosition& tp = *p++;
 if (!tp.Parse(pReader, pos, size)) {
 return false;
 }
 }

    pos += size;  
 if (pos > stop)
 return false;
 }

  assert(size_t(p - m_track_positions) == m_track_positions_count);

  m_element_start = element_start;
  m_element_size = element_size;

 return true;
}
