copy_class(mrb_state *mrb, mrb_value dst, mrb_value src)
{
  struct RClass *dc = mrb_class_ptr(dst);
  struct RClass *sc = mrb_class_ptr(src);
   
  if (sc->flags & MRB_FLAG_IS_PREPENDED) {
    struct RClass *c0 = sc->super;
    struct RClass *c1 = dc;

     
    while (!(c0->flags & MRB_FLAG_IS_ORIGIN)) {
      c1->super = mrb_class_ptr(mrb_obj_dup(mrb, mrb_obj_value(c0)));
      c1 = c1->super;
      c0 = c0->super;
    }
    c1->super = mrb_class_ptr(mrb_obj_dup(mrb, mrb_obj_value(c0)));
    c1->super->flags |= MRB_FLAG_IS_ORIGIN;
  }
  if (sc->mt) {
    dc->mt = kh_copy(mt, mrb, sc->mt);
  }
  else {
    dc->mt = kh_init(mt, mrb);
  }
  dc->super = sc->super;
  MRB_SET_INSTANCE_TT(dc, MRB_INSTANCE_TT(sc));
}
