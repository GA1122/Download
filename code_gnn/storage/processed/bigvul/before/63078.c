static int __init early_platform_driver_probe_id(char *class_str,
						 int id,
						 int nr_probe)
{
	struct early_platform_driver *epdrv;
	struct platform_device *match;
	int match_id;
	int n = 0;
	int left = 0;

	list_for_each_entry(epdrv, &early_platform_driver_list, list) {
		 
		if (strcmp(class_str, epdrv->class_str))
			continue;

		if (id == -2) {
			match_id = epdrv->requested_id;
			left = 1;

		} else {
			match_id = id;
			left += early_platform_left(epdrv, id);

			 
			switch (epdrv->requested_id) {
			case EARLY_PLATFORM_ID_ERROR:
			case EARLY_PLATFORM_ID_UNSET:
				break;
			default:
				if (epdrv->requested_id == id)
					match_id = EARLY_PLATFORM_ID_UNSET;
			}
		}

		switch (match_id) {
		case EARLY_PLATFORM_ID_ERROR:
			pr_warn("%s: unable to parse %s parameter\n",
				class_str, epdrv->pdrv->driver.name);
			 
		case EARLY_PLATFORM_ID_UNSET:
			match = NULL;
			break;
		default:
			match = early_platform_match(epdrv, match_id);
		}

		if (match) {
			 
			if (!match->dev.init_name && slab_is_available()) {
				if (match->id != -1)
					match->dev.init_name =
						kasprintf(GFP_KERNEL, "%s.%d",
							  match->name,
							  match->id);
				else
					match->dev.init_name =
						kasprintf(GFP_KERNEL, "%s",
							  match->name);

				if (!match->dev.init_name)
					return -ENOMEM;
			}

			if (epdrv->pdrv->probe(match))
				pr_warn("%s: unable to probe %s early.\n",
					class_str, match->name);
			else
				n++;
		}

		if (n >= nr_probe)
			break;
	}

	if (left)
		return n;
	else
		return -ENODEV;
}