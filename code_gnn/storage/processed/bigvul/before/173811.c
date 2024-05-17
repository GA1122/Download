int SoftGSM::DecodeGSM(gsm handle,
 int16_t *out, uint8_t *in, size_t inSize) {

 int ret = 0;
 while (inSize > 0) {
        gsm_decode(handle, in, out);
        in += 33;
        inSize -= 33;
        out += 160;
        ret += 160;
        gsm_decode(handle, in, out);
        in += 32;
        inSize -= 32;
        out += 160;
        ret += 160;
 }
 return ret;
}
