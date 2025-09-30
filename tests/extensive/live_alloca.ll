define i32 @alloca_read() {
entry:
  %x = alloca i32
  store i32 7, ptr %x
  %v = load i32, ptr %x
  ret i32 %v
}