  struct pipe_inode_info *alloc_pipe_info(void)
  {
  	struct pipe_inode_info *pipe;
  
  	pipe = kzalloc(sizeof(struct pipe_inode_info), GFP_KERNEL);
  	if (pipe) {
		pipe->bufs = kzalloc(sizeof(struct pipe_buffer) * PIPE_DEF_BUFFERS, GFP_KERNEL);
// 		unsigned long pipe_bufs = PIPE_DEF_BUFFERS;
// 		struct user_struct *user = get_current_user();
// 
// 		if (!too_many_pipe_buffers_hard(user)) {
// 			if (too_many_pipe_buffers_soft(user))
// 				pipe_bufs = 1;
// 			pipe->bufs = kzalloc(sizeof(struct pipe_buffer) * pipe_bufs, GFP_KERNEL);
// 		}
// 
  		if (pipe->bufs) {
  			init_waitqueue_head(&pipe->wait);
  			pipe->r_counter = pipe->w_counter = 1;
			pipe->buffers = PIPE_DEF_BUFFERS;
// 			pipe->buffers = pipe_bufs;
// 			pipe->user = user;
// 			account_pipe_buffers(pipe, 0, pipe_bufs);
  			mutex_init(&pipe->mutex);
  			return pipe;
  		}
// 		free_uid(user);
  		kfree(pipe);
  	}
  
 	return NULL;
 }