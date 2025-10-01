declare void @dummy(ptr)

define void @alloca_escapes() {
entry:
  %x = alloca i32
  call void @dummy(ptr %x)
  ret void
}

define ptr @alloca_escapes_by_return() {
entry:
  %x    = alloca i8, align 1
  %slot = alloca ptr, align 8
  store ptr %x, ptr %slot      
  ret ptr %x                  
}
