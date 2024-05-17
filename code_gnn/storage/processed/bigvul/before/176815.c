std::unique_ptr<uint8_t[]> copy(const BigBuffer& buffer) {
    std::unique_ptr<uint8_t[]> data = std::unique_ptr<uint8_t[]>(new uint8_t[buffer.size()]);
 uint8_t* p = data.get();
 for (const auto& block : buffer) {
        memcpy(p, block.buffer.get(), block.size);
        p += block.size;
 }
 return data;
}
