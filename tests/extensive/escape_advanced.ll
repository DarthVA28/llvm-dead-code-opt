define i32 @store_through_slot() {
entry:
  %x    = alloca i32, align 4      
  %slot = alloca ptr, align 8         
  %y    = alloca i32, align 4         

  store i32 7, ptr %x, align 4
  store ptr %y, ptr %slot, align 8

  %p  = load ptr, ptr %slot, align 8  
  %vx = load i32, ptr %x, align 4     
  store i32 %vx, ptr %p, align 4      

  %ry = load i32, ptr %y, align 4
  ret i32 %ry
}

