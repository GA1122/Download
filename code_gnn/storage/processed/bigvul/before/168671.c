void BindButtonColors(Model* model, V* view, C colors, S setter) {
  view->AddBinding(base::MakeUnique<Binding<ButtonColors>>(
      base::Bind([](Model* m, C c) { return (m->color_scheme()).*c; },
                 base::Unretained(model), colors),
      base::Bind([](V* v, S s, const ButtonColors& value) { (v->*s)(value); },
                 base::Unretained(view), setter)));
}
