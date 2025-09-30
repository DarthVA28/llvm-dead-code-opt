define i64 @ptrtoint() {
entry:
  %x = alloca i32
  %i = ptrtoint ptr %x to i64
  ret i64 %i
}
