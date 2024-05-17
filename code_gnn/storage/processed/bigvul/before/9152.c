vrend_so_target_reference(struct vrend_so_target **ptr, struct vrend_so_target *target)
{
   struct vrend_so_target *old_target = *ptr;

   if (pipe_reference(&(*ptr)->reference, &target->reference))
      vrend_destroy_so_target(old_target);
   *ptr = target;
}
