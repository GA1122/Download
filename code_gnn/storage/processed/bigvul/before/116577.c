  base::Closure NewUnderflowClosure() {
    return base::Bind(&AudioRendererImplTest::OnUnderflow,
                      base::Unretained(this));
  }
