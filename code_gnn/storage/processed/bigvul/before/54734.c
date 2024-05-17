static ssize_t snd_seq_write(struct file *file, const char __user *buf,
			     size_t count, loff_t *offset)
{
	struct snd_seq_client *client = file->private_data;
	int written = 0, len;
	int err = -EINVAL;
	struct snd_seq_event event;

	if (!(snd_seq_file_flags(file) & SNDRV_SEQ_LFLG_OUTPUT))
		return -ENXIO;

	 
	if (snd_BUG_ON(!client))
		return -ENXIO;
		
	if (!client->accept_output || client->pool == NULL)
		return -ENXIO;

	  
	if (client->pool->size > 0 && !snd_seq_write_pool_allocated(client)) {
		if (snd_seq_pool_init(client->pool) < 0)
			return -ENOMEM;
	}

	 
	while (count >= sizeof(struct snd_seq_event)) {
		 
		len = sizeof(event);
		if (copy_from_user(&event, buf, len)) {
			err = -EFAULT;
			break;
		}
		event.source.client = client->number;	 
		 
		if (check_event_type_and_length(&event)) {
			err = -EINVAL;
			break;
		}

		 
		if (event.type == SNDRV_SEQ_EVENT_NONE)
			goto __skip_event;
		else if (snd_seq_ev_is_reserved(&event)) {
			err = -EINVAL;
			break;
		}

		if (snd_seq_ev_is_variable(&event)) {
			int extlen = event.data.ext.len & ~SNDRV_SEQ_EXT_MASK;
			if ((size_t)(extlen + len) > count) {
				 
				err = -EINVAL;
				break;
			}
			 
			event.data.ext.len = extlen | SNDRV_SEQ_EXT_USRPTR;
			event.data.ext.ptr = (char __force *)buf
						+ sizeof(struct snd_seq_event);
			len += extlen;  
		} else {
#ifdef CONFIG_COMPAT
			if (client->convert32 && snd_seq_ev_is_varusr(&event)) {
				void *ptr = (void __force *)compat_ptr(event.data.raw32.d[1]);
				event.data.ext.ptr = ptr;
			}
#endif
		}

		 
		err = snd_seq_client_enqueue_event(client, &event, file,
						   !(file->f_flags & O_NONBLOCK),
						   0, 0);
		if (err < 0)
			break;

	__skip_event:
		 
		count -= len;
		buf += len;
		written += len;
	}

	return written ? written : err;
}
