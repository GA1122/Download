void arg_show_usage(FILE *fp, const struct arg_def *const *defs) {
 char option_text[40] = {0};

 for (; *defs; defs++) {
 const struct arg_def *def = *defs;
 char *short_val = def->has_val ? " <arg>" : "";
 char *long_val = def->has_val ? "=<arg>" : "";

 if (def->short_name && def->long_name) {
 char *comma = def->has_val ? "," : ",      ";

      snprintf(option_text, 37, "-%s%s%s --%s%6s",
               def->short_name, short_val, comma,
               def->long_name, long_val);
 } else if (def->short_name)
      snprintf(option_text, 37, "-%s%s",
               def->short_name, short_val);
 else if (def->long_name)
      snprintf(option_text, 37, "          --%s%s",
               def->long_name, long_val);

    fprintf(fp, "  %-37s\t%s\n", option_text, def->desc);

 if (def->enums) {
 const struct arg_enum_list *listptr;

      fprintf(fp, "  %-37s\t  ", "");

 for (listptr = def->enums; listptr->name; listptr++)
        fprintf(fp, "%s%s", listptr->name,
                listptr[1].name ? ", " : "\n");
 }
 }
}
