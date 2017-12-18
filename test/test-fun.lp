; fun
(fun {fun-test-fun x y} {+ x y})
(assert-defined {fun-test-fun})
(assert-equal 5 (fun-test-fun 2 3))

; unpack
(assert-equal (unpack + {5 6 7}) 18)

; pack
(assert-equal (pack head 5 6 7) {5})

; curry
(assert-equal curry unpack)

; FAIL
(assert-true false)

; uncurry
(assert-equal uncurry pack)

; do
(def {fun-test-x} nil)
(def {fun-test-y} nil)
(def {fun-test-z} nil)
(do
  (def {fun-test-x} 2)
  (def {fun-test-y} 3)
  (def {fun-test-x} 4)
  (def {fun-test-z} 5))
(assert-equal 4 fun-test-x)
(assert-equal 3 fun-test-y)
(assert-equal 5 fun-test-z)

; let
(assert-undefined {fun-test-u})
(assert-equal (
  let {do (= {fun-test-fun} 100)
    (fun-test-fun)
  })
100)
(assert-undefined {fun-test-u})

; flip
(def {fun-test-flip} nil)
((flip def) 100 {fun-test-flip})
(assert-equal 100 fun-test-flip)

; comp
(assert-equal ((comp - (unpack *)) {2 8}) -16)
