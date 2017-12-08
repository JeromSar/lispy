;
; Mathematical functions
;

(def {$pi} 3.14159)
(def {$tau} (* $pi 2))

; Absolute value
(fun {abs n} {
  if (< n 0) {- n} {n}
})

; Sum
(fun {sum & n} {
  list-sum n
})

; Product
(fun {product & n} {
  list-product n
})

; Average
(fun {avg & n} {
  list-avg n
})

; Square root
(fun {sqrt x} {
  if (< x 0) {
    error "Cannot calculate negative root"
  } {
    if (== x 0) {
      0.0
    } {
      sqrt-iter (/ x 2.0) x
    }
  }
})

; Square root helper
(fun {sqrt-iter guess x} {
  if (< (abs (- (* guess guess) x)) (* x 0.00001)) {
    guess
  } {
    sqrt-iter ((avg guess (/ x guess))) x
  }
})

; Fibonacci
(fun {fib n} {
  select
    { (== n 0) 0 }
    { (== n 1) 1 }
    { otherwise (+ (fib (- n 1)) (fib (- n 2))) }
})
