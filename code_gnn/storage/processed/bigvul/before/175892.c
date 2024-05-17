void logIov(const std::vector<iovec>& iov) {
 for (const iovec& row : iov) {
        logHex(0, reinterpret_cast<char*>(row.iov_base), row.iov_len);
 }
}
