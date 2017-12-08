; map-new
(assert-equal {}
    (map-new nil))
(assert-equal {1 "Hello" 2 "World" 3 "Test!"}
    (map-new {1 "Hello"} {2 "World"} {3 "Test!"}))

; map-len
(assert-equal 0
    (map-len(map-new nil)))
(assert-equal 1
    (map-len(map-new {1 "Hello"})))
(assert-equal 2
    (map-len(map-new {1 "Hello"} {2 "World"} {3 "Test"})))

; map-put
(assert-equal {1 2}
    (map-put (map-new nil) 1 2))
(assert-equal {1 2 3 4 5 6}
    (map-put (map-new {1 2} {3 4}) 5 6))
(assert-equal {1 "Hello" 3 4 5 6}
    (map-put (map-new {1 2} {3 4}) 1 "Hello"))
(assert-equal {1 2 3 "Hello" 5 6}
    (map-put (map-new {1 2} {3 4}) 3 "Hello"))
(assert-equal {1 2 3 4 5 "Hello"}
    (map-put (map-new {1 2} {3 4}) 3 "Hello"))

; map-putall
(assert-equal {1 2 3 4 5 6}
    (map-putall (map-new nil) {1 2} {3 4} {5 6}))
(assert-equal {1 2 5 6 3 4}
    (map-putall (map-new {1 "Hello"} {5 2}) {1 2} {3 4} {5 6}))

; map-get
(assert-equal "Hello"
    (map-get (map-new {5 "Hello"} {6 "World"} {7 "Test!"}) 5))
(assert-equal "World"
    (map-get (map-new {5 "Hello"} {6 "World"} {7 "Test!"}) 6))
(assert-equal "Test!"
    (map-get (map-new {5 "Hello"} {6 "World"} {7 "Test!"}) 6))
(assert-equal nil
    (map-get (map-new {5 "Hello"} {6 "World"} {7 "Test!"}) 10))

; map-has
(assert-true
    (map-has (map-new {1 "Hello"} {2 "World"} {3 "Test!"})) 1)
(assert-true
    (map-has (map-new {1 "Hello"} {2 "World"} {3 "Test!"})) 2)
(assert-true
    (map-has (map-new {1 "Hello"} {2 "World"} {3 "Test!"})) 3)
(assert-false
    (map-has (map-new {1 "Hello"} {2 "World"} {3 "Test!"})) 4)
(assert-true
    (map-has (map-new {1 "Hello"} {"World" 2} {3 "Test!"})) "World")
(assert-false
    (map-has (map-new {1 "Hello"} {"World" 2} {3 "Test!"})) "world")

; map-del
(assert-equal {2 "World" 3 "Test!"}
    (map-del (map-new {1 "Hello"} {2 "World"} {3 "Test!"}) 1))
(assert-equal {1 "Hello" 3 "Test!"}
    (map-del (map-new {1 "Hello"} {2 "World"} {3 "Test!"}) 2))
(assert-equal {1 "Hello" 2 "World!"}
    (map-del (map-new {1 "Hello"} {2 "World"} {3 "Test!"}) 3))
(assert-equal {1 "Hello" 2 "World!" 3 "Test!"}
    (map-del (map-new {1 "Hello"} {2 "World"} {3 "Test!"}) 4))

; map-keys
(assert-equal {}
    (map-keys (map-new nil)))
(assert-equal {1 2 3}
    (map-keys (map-new {1 "Hello"} {2 "World"} {3 "Test!"})))
(assert-equal {"Hello" "World" "Test!"}
    (map-keys (map-new {"Hello" 1} {"World" 2} {"Test!" 3})))

; map-values
(assert-equal {}
    (map-values (map-new nil)))
(assert-equal {"Hello" "World" "Test!"}
    (map-values (map-new {1 "Hello"} {2 "World"} {3 "Test!"})))
(assert-equal {1 2 3}
    (map-values (map-new {"Hello" 1} {"World" 2} {"Test!" 3})))

; map-foreach
(def {map-test} 1)
(map-foreach (map-new {1 2} {2 4} {4 8})
    (\ {k v} { do
        (assert-equal map-test k)
        (assert-equal v (* k 2))
        (def {map-test} v)
    }))
(assert-equal 8 map-test)
