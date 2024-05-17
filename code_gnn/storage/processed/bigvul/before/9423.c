static void ssl_check_for_safari(SSL *s, const PACKET *pkt)
{
    unsigned int type;
    PACKET sni, tmppkt;
    size_t ext_len;

    static const unsigned char kSafariExtensionsBlock[] = {
        0x00, 0x0a,              
        0x00, 0x08,              
        0x00, 0x06,              
        0x00, 0x17,              
        0x00, 0x18,              
        0x00, 0x19,              

        0x00, 0x0b,              
        0x00, 0x02,              
        0x01,                    
        0x00,                    
         
        0x00, 0x0d,              
        0x00, 0x0c,              
        0x00, 0x0a,              
        0x05, 0x01,              
        0x04, 0x01,              
        0x02, 0x01,              
        0x04, 0x03,              
        0x02, 0x03,              
    };

     
    static const size_t kSafariCommonExtensionsLength = 18;

    tmppkt = *pkt;

    if (!PACKET_forward(&tmppkt, 2)
        || !PACKET_get_net_2(&tmppkt, &type)
        || !PACKET_get_length_prefixed_2(&tmppkt, &sni)) {
        return;
    }

    if (type != TLSEXT_TYPE_server_name)
        return;

    ext_len = TLS1_get_client_version(s) >= TLS1_2_VERSION ?
        sizeof(kSafariExtensionsBlock) : kSafariCommonExtensionsLength;

    s->s3->is_probably_safari = PACKET_equal(&tmppkt, kSafariExtensionsBlock,
                                             ext_len);
}
