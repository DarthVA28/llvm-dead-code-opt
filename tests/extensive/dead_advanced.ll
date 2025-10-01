define i32 @store_pos() {
entry:
  %x    = alloca i32, align 4         ; int x
  %slot = alloca ptr, align 8         ; int *slot
  %y    = alloca i32, align 4         ; int y

  store i32 7, ptr %x, align 4
  store ptr %y, ptr %slot, align 8

  ; note from v: this version will be optimized 
  ; since we don't go through %p
  %p  = load ptr, ptr %slot, align 8  
  %vx = load i32, ptr %x, align 4     
  store i32 %vx, ptr %y, align 4      

  ret i32 0
}



define i32 @store_neg() {
entry:
  %x    = alloca i32, align 4         ; int x
  %slot = alloca ptr, align 8         ; int *slot
  %y    = alloca i32, align 4         ; int y

  store i32 7, ptr %x, align 4
  store ptr %y, ptr %slot, align 8

  ; note from v: this version won't be optimized 
  ; since we need to perform memory analysis to know that p points to y
  %p  = load ptr, ptr %slot, align 8  
  %vx = load i32, ptr %x, align 4     
  store i32 %vx, ptr %p, align 4      

  ret i32 0
}

