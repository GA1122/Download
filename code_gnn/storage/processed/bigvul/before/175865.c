int XfrmController::fillNlAttrXfrmAlgoAead(const XfrmAlgo& inAlgo, nlattr_algo_aead* algo) {
 if (inAlgo.name.empty()) {  
 return 0;
 }

 int len = NLA_HDRLEN + sizeof(xfrm_algo_aead);
    strncpy(algo->aead.alg_name, inAlgo.name.c_str(), sizeof(algo->aead.alg_name));
    algo->aead.alg_key_len = inAlgo.key.size() * 8;  

    algo->aead.alg_icv_len = inAlgo.truncLenBits;

    memcpy(algo->key, &inAlgo.key[0], inAlgo.key.size());
    len += inAlgo.key.size();

    fillXfrmNlaHdr(&algo->hdr, XFRMA_ALG_AEAD, len);
 return len;
}
