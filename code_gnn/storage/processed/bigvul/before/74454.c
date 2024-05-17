static int _regulator_enable(struct regulator_dev *rdev)
{
	int ret;

	 
	if (rdev->constraints &&
	    (rdev->constraints->valid_ops_mask & REGULATOR_CHANGE_DRMS))
		drms_uA_update(rdev);

	if (rdev->use_count == 0) {
		 
		ret = _regulator_is_enabled(rdev);
		if (ret == -EINVAL || ret == 0) {
			if (!_regulator_can_change_status(rdev))
				return -EPERM;

			ret = _regulator_do_enable(rdev);
			if (ret < 0)
				return ret;

		} else if (ret < 0) {
			rdev_err(rdev, "is_enabled() failed: %d\n", ret);
			return ret;
		}
		 
	}

	rdev->use_count++;

	return 0;
}