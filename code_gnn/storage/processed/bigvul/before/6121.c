static int server_hello(SSL *s)
{
    unsigned char *p, *d;
    int n, hit;

    p = (unsigned char *)s->init_buf->data;
    if (s->state == SSL2_ST_SEND_SERVER_HELLO_A) {
        d = p + 11;
        *(p++) = SSL2_MT_SERVER_HELLO;  
        hit = s->hit;
        *(p++) = (unsigned char)hit;
# if 1
        if (!hit) {
            if (s->session->sess_cert != NULL)
                 
                ssl_sess_cert_free(s->session->sess_cert);
            s->session->sess_cert = ssl_sess_cert_new();
            if (s->session->sess_cert == NULL) {
                SSLerr(SSL_F_SERVER_HELLO, ERR_R_MALLOC_FAILURE);
                return (-1);
            }
        }
         

# else                           
        if (!hit) {              
            CRYPTO_add(&s->cert->references, 1, CRYPTO_LOCK_SSL_CERT);
            if (s->session->sess_cert != NULL)
                ssl_cert_free(s->session->sess_cert);
            s->session->sess_cert = s->cert;
        } else {                 
            if (s->session->sess_cert == NULL) {
                CRYPTO_add(&s->cert->references, 1, CRYPTO_LOCK_SSL_CERT);
                s->session->sess_cert = s->cert;
            }
        }
# endif

        if (s->cert == NULL) {
            ssl2_return_error(s, SSL2_PE_NO_CERTIFICATE);
            SSLerr(SSL_F_SERVER_HELLO, SSL_R_NO_CERTIFICATE_SPECIFIED);
            return (-1);
        }

        if (hit) {
            *(p++) = 0;          
            s2n(s->version, p);  
            s2n(0, p);           
            s2n(0, p);           
        } else {
             
             
            *(p++) = SSL2_CT_X509_CERTIFICATE;
            s2n(s->version, p);  
            n = i2d_X509(s->cert->pkeys[SSL_PKEY_RSA_ENC].x509, NULL);
            s2n(n, p);           
            i2d_X509(s->cert->pkeys[SSL_PKEY_RSA_ENC].x509, &d);
            n = 0;

             
            n = ssl_cipher_list_to_bytes(s, s->session->ciphers, d, 0);
            d += n;
            s2n(n, p);           
        }

         
        s2n(SSL2_CONNECTION_ID_LENGTH, p);  
        s->s2->conn_id_length = SSL2_CONNECTION_ID_LENGTH;
        if (RAND_pseudo_bytes(s->s2->conn_id, (int)s->s2->conn_id_length) <=
            0)
            return -1;
        memcpy(d, s->s2->conn_id, SSL2_CONNECTION_ID_LENGTH);
        d += SSL2_CONNECTION_ID_LENGTH;

        s->state = SSL2_ST_SEND_SERVER_HELLO_B;
        s->init_num = d - (unsigned char *)s->init_buf->data;
        s->init_off = 0;
    }
     
     
    if (s->hit) {
        if (!ssl_init_wbio_buffer(s, 1))
            return (-1);
    }

    return (ssl2_do_write(s));
}
