; $pi
(assert-defined {$pi})

; $tau
(assert-defined {$tau})
(assert-equal (* $pi 2) $tau)

; abs
(assert-equal 3 (abs 3))
(assert-equal 3 (abs -3))
(assert-equal 0 (abs 0))
(assert-equal 3.0 (abs 3.0))
(assert-equal 3.0 (abs -3.0))
(assert-equal 0.0 (abs 0.0))

; sum
(assert-equal 6 (sum 1 2 3))
(assert-equal 6.0 (sum 1.0 2 3))
(assert-equal 6.0 (sum 1.0 2 3 -1 1))

; product
(assert-equal 24 (product 1 2 3 4))
(assert-equal 24 (product -1 -2 -3 -4))
(assert-equal 24.0 (product -1.0 -2.0 -3.0 -4.0))

; avg
(assert-equal 2 (avg 2 1 3))
(assert-equal 2 (avg 2 1 2 3 2))
(assert-equal 3 (avg 3 3 2 4))
(assert-equal 0 (avg -1 1 0 0 0 1 -2 1))

; sqrt
(assert-error {sqrt -2})
(assert-error {sqrt -1})
(assert-float-equal 0.0 (sqrt 0))
(assert-float-equal 1.0 (sqrt 1))
(assert-float-equal 2.0 (sqrt 4))
(assert-float-equal 3.0 (sqrt 9))

; fib
(assert-equal 0 (fib 0))
(assert-equal 1 (fib 1))
(assert-equal 1 (fib 2))
(assert-equal 2 (fib 3))
(assert-equal 3 (fib 4))
(assert-equal 5 (fib 5))
(assert-equal 8 (fib 6))
(assert-equal 13 (fib 7))
