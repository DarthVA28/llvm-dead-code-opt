declare void @dummy(ptr)

define void @alloca_escapes() {
entry:
  %x = alloca i32
  call void @dummy(ptr %x)
  ret void
}
