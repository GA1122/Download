 static int skcipher_recvmsg(struct kiocb *unused, struct socket *sock,
 			    struct msghdr *msg, size_t ignored, int flags)
 {
 	struct sock *sk = sock->sk;
 	struct alg_sock *ask = alg_sk(sk);
 	struct skcipher_ctx *ctx = ask->private;
 	unsigned bs = crypto_ablkcipher_blocksize(crypto_ablkcipher_reqtfm(
 		&ctx->req));
 	struct skcipher_sg_list *sgl;
 	struct scatterlist *sg;
 	unsigned long iovlen;
 	struct iovec *iov;
 	int err = -EAGAIN;
 	int used;
  	long copied = 0;
  
  	lock_sock(sk);
	msg->msg_namelen = 0;
  	for (iov = msg->msg_iov, iovlen = msg->msg_iovlen; iovlen > 0;
  	     iovlen--, iov++) {
  		unsigned long seglen = iov->iov_len;
 		char __user *from = iov->iov_base;
 
 		while (seglen) {
 			sgl = list_first_entry(&ctx->tsgl,
 					       struct skcipher_sg_list, list);
 			sg = sgl->sg;
 
 			while (!sg->length)
 				sg++;
 
 			used = ctx->used;
 			if (!used) {
 				err = skcipher_wait_for_data(sk, flags);
 				if (err)
 					goto unlock;
 			}
 
 			used = min_t(unsigned long, used, seglen);
 
 			used = af_alg_make_sg(&ctx->rsgl, from, used, 1);
 			err = used;
 			if (err < 0)
 				goto unlock;
 
 			if (ctx->more || used < ctx->used)
 				used -= used % bs;
 
 			err = -EINVAL;
 			if (!used)
 				goto free;
 
 			ablkcipher_request_set_crypt(&ctx->req, sg,
 						     ctx->rsgl.sg, used,
 						     ctx->iv);
 
 			err = af_alg_wait_for_completion(
 				ctx->enc ?
 					crypto_ablkcipher_encrypt(&ctx->req) :
 					crypto_ablkcipher_decrypt(&ctx->req),
 				&ctx->completion);
 
 free:
 			af_alg_free_sg(&ctx->rsgl);
 
 			if (err)
 				goto unlock;
 
 			copied += used;
 			from += used;
 			seglen -= used;
 			skcipher_pull_sgl(sk, used);
 		}
 	}
 
 	err = 0;
 
 unlock:
 	skcipher_wmem_wakeup(sk);
 	release_sock(sk);
 
 	return copied ?: err;
 }