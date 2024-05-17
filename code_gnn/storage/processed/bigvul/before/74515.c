static int __init regulator_init_complete(void)
{
	struct regulator_dev *rdev;
	const struct regulator_ops *ops;
	struct regulation_constraints *c;
	int enabled, ret;

	 
	if (of_have_populated_dt())
		has_full_constraints = true;

	mutex_lock(&regulator_list_mutex);

	 
	list_for_each_entry(rdev, &regulator_list, list) {
		ops = rdev->desc->ops;
		c = rdev->constraints;

		if (c && c->always_on)
			continue;

		if (c && !(c->valid_ops_mask & REGULATOR_CHANGE_STATUS))
			continue;

		mutex_lock(&rdev->mutex);

		if (rdev->use_count)
			goto unlock;

		 
		if (ops->is_enabled)
			enabled = ops->is_enabled(rdev);
		else
			enabled = 1;

		if (!enabled)
			goto unlock;

		if (have_full_constraints()) {
			 
			rdev_info(rdev, "disabling\n");
			ret = _regulator_do_disable(rdev);
			if (ret != 0)
				rdev_err(rdev, "couldn't disable: %d\n", ret);
		} else {
			 
			rdev_warn(rdev, "incomplete constraints, leaving on\n");
		}

unlock:
		mutex_unlock(&rdev->mutex);
	}

	mutex_unlock(&regulator_list_mutex);

	return 0;
}
