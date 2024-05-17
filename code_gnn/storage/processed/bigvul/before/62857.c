static void punt_bios_to_rescuer(struct bio_set *bs)
{
	struct bio_list punt, nopunt;
	struct bio *bio;

	if (WARN_ON_ONCE(!bs->rescue_workqueue))
		return;
	 

	bio_list_init(&punt);
	bio_list_init(&nopunt);

	while ((bio = bio_list_pop(&current->bio_list[0])))
		bio_list_add(bio->bi_pool == bs ? &punt : &nopunt, bio);
	current->bio_list[0] = nopunt;

	bio_list_init(&nopunt);
	while ((bio = bio_list_pop(&current->bio_list[1])))
		bio_list_add(bio->bi_pool == bs ? &punt : &nopunt, bio);
	current->bio_list[1] = nopunt;

	spin_lock(&bs->rescue_lock);
	bio_list_merge(&bs->rescue_list, &punt);
	spin_unlock(&bs->rescue_lock);

	queue_work(bs->rescue_workqueue, &bs->rescue_work);
}