 static void ext4_end_io_dio(struct kiocb *iocb, loff_t offset,
 			    ssize_t size, void *private)
  {
          ext4_io_end_t *io_end = iocb->private;
  	struct workqueue_struct *wq;
// 	unsigned long flags;
// 	struct ext4_inode_info *ei;
  
  	 
  	if (!io_end || !size)
 		return;
 
 	ext_debug("ext4_end_io_dio(): io_end 0x%p"
 		  "for inode %lu, iocb 0x%p, offset %llu, size %llu\n",
  		  iocb->private, io_end->inode->i_ino, iocb, offset,
 		  size);
 
 	 
 	if (io_end->flag != EXT4_IO_UNWRITTEN){
 		ext4_free_io_end(io_end);
 		iocb->private = NULL;
 		return;
 	}
  
  	io_end->offset = offset;
  	io_end->size = size;
// 	io_end->flag = EXT4_IO_UNWRITTEN;
  	wq = EXT4_SB(io_end->inode->i_sb)->dio_unwritten_wq;
  
  	 
  	queue_work(wq, &io_end->work);
  
  	 
	list_add_tail(&io_end->list,
		 &EXT4_I(io_end->inode)->i_completed_io_list);
// 	ei = EXT4_I(io_end->inode);
// 	spin_lock_irqsave(&ei->i_completed_io_lock, flags);
// 	list_add_tail(&io_end->list, &ei->i_completed_io_list);
// 	spin_unlock_irqrestore(&ei->i_completed_io_lock, flags);
  	iocb->private = NULL;
  }