rs_filter_set_recursive(RSFilter *filter, ...)
{
	va_list ap;
	gchar *property_name;
	RSFilter *current_filter;
	GParamSpec *spec;
	RSFilter *first_seen_here = NULL;
	GTypeValueTable *table = NULL;
	GType type = 0;
	union CValue {
		gint     v_int;
		glong    v_long;
		gint64   v_int64;
		gdouble  v_double;
		gpointer v_pointer;
	} value;

	g_return_if_fail(RS_IS_FILTER(filter));

	va_start(ap, filter);

	 
	while ((property_name = va_arg(ap, gchar *)))
	{
		 
		table = NULL;

		current_filter = filter;
		 
		do {
			if ((spec = g_object_class_find_property(G_OBJECT_GET_CLASS(current_filter), property_name)))
				if (spec->flags & G_PARAM_WRITABLE)
				{
					 
					if (!table)
					{
						first_seen_here = current_filter;
						type = spec->value_type;
						table = g_type_value_table_peek(type);

						 
						if (!table)
							g_error("No GTypeValueTable found for '%s'", g_type_name(type));

						switch (table->collect_format[0])
						{
							case 'i': value.v_int = va_arg(ap, gint); break;
							case 'l': value.v_long = va_arg(ap, glong); break;
							case 'd': value.v_double = va_arg(ap, gdouble); break;
							case 'p': value.v_pointer = va_arg(ap, gpointer); break;
							default: g_error("Don't know how to collect for '%s'", g_type_name(type)); break;
						}
					}

					if (table)
					{
						 
						if (type != spec->value_type)
							g_warning("Diverging types found for property '%s' (on filter '%s' and '%s')",
								property_name,
								RS_FILTER_NAME(first_seen_here),
								RS_FILTER_NAME(current_filter));

						switch (table->collect_format[0])
						{
							case 'i': g_object_set(current_filter, property_name, value.v_int, NULL); break;
							case 'l': g_object_set(current_filter, property_name, value.v_long, NULL); break;
							case 'd': g_object_set(current_filter, property_name, value.v_double, NULL); break;
							case 'p': g_object_set(current_filter, property_name, value.v_pointer, NULL); break;
							default: break;
						}
					}
				}
		} while (RS_IS_FILTER(current_filter = current_filter->previous));
		if (!table)
		{
			va_end(ap);
			return;
		}
	}

	va_end(ap);
}
