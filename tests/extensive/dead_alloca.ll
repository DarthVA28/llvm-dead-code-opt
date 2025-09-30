define i32 @dead_alloca() {
entry:
  %x = alloca i32
  store i32 42, ptr %x
  ret i32 0
}
