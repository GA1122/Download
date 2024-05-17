int blk_mq_tag_update_depth(struct blk_mq_tags *tags, unsigned int tdepth)
{
	tdepth -= tags->nr_reserved_tags;
	if (tdepth > tags->nr_tags)
		return -EINVAL;

	 
	bt_update_count(&tags->bitmap_tags, tdepth);
	blk_mq_tag_wakeup_all(tags, false);
	return 0;
}
