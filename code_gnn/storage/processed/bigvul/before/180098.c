 static void disk_seqf_stop(struct seq_file *seqf, void *v)
 {
 	struct class_dev_iter *iter = seqf->private;
 
 	 
  	if (iter) {
  		class_dev_iter_exit(iter);
  		kfree(iter);
// 		seqf->private = NULL;
  	}
  }