define void @volatile() {
entry:
  %x = alloca i32
  store volatile i32 1, ptr %x
  ret void
}