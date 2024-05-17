  bool WebMediaPlayerImpl::HasSingleSecurityOrigin() const {
//   if (demuxer_found_hls_) {
//      
//      
//     return false;
//   }
// 
    if (data_source_)
      return data_source_->HasSingleOrigin();
    return true;
 }