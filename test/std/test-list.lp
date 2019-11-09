; fst
(assert-equal 1 (fst {1 2 3}))
(assert-equal 1 (fst {1}))
(assert-equal "hi" (fst {"hi" (error "fail")}))
(assert-error {fst {}})

; snd
(assert-equal 2 (snd {1 2 3}))
(assert-equal 2 (snd {1 2}))
(assert-equal "hi" (snd {(error "fail") "hi"}))
(assert-error {snd {}})

; trd
(assert-equal 3 (trd {1 2 3}))
(assert-equal "hi" (trd {(error "fail") (error "fail") "hi"}))
(assert-error {trd {}})

; nth
(assert-equal 5 (nth 0 {5 6 7}))
(assert-equal 6 (nth 1 {5 6 7}))
(assert-equal 7 (nth 2 {5 6 7}))
(assert-equal "hi" (nth 1 {(error "fail") "hi" (error "fail")}))
(assert-error {nth 0 {}})
(assert-error {nth 1 {1}})

; last
(assert-equal 3 (last {1 2 3}))
(assert-error {last {}})

; take
(assert-equal {1 2 3} (take 3 {1 2 3 4 5}))
(assert-error {take 1 {}})

; drop
(assert-equal {3 4 5} (drop 2 {1 2 3 4 5}))
(assert-equal {1 2 3} (drop 0 {1 2 3}))
(assert-error {drop 1 {}})

; split
(assert-equal {{1 2 3} {4 5}} (split 3 {1 2 3 4 5}))
(assert-equal {{} {1 2 3 4 5}} (split 0 {1 2 3 4 5}))
(assert-equal {{1 2 3 4 5} {}} (split 5 {1 2 3 4 5}))
(assert-error {split 1 {}})

; elem
(assert-true (elem 1 {1 2 3}))
(assert-true (elem 2 {1 2 3}))
(assert-true (elem 3 {1 2 3}))
(assert-false (elem 4 {1 2 3}))

; map
(assert-equal {-1 -2 -3} (map - {1 2 3}))
(assert-equal {2 4 6} (map (\ {x} {* x 2}) {1 2 3}))
(assert-equal {} (map - {}))

; filter
(assert-equal {1 2 3} (filter (\ {x} {< x 6}) {1 15 33 2 45 6 62 3 9}))
(assert-equal {} (filter (\ {x} {0}) {1 2 3}))
(assert-equal {1 2 3} (filter (\ {x} {1}) {1 2 3}))
(assert-equal {} (filter (\ {x} {1}) {}))

; foldl
(assert-equal 8 (foldl + 2 {1 2 3})) 
(assert-equal "Hello, world!" (foldl join "Hello, " {"world" "!"})) 
(assert-equal "Hello, world!" (foldl join "Hello, world!" {}))

; foldr
(assert-equal 8 (foldr + 2 {1 2 3})) 
(assert-equal "Hello, world!" (foldr join "Hello, " {"!" "world"})) 
(assert-equal "Hello, world!" (foldr join "Hello, world!" {}))

; foreach
(def {test-list-x} nil)
(def {test-list-y} nil)
(def {test-list-z} nil)
(foreach {{test-list-x} {test-list-y} {test-list-z}} (\ {x} {def x 1}))
(assert-equal 1 test-list-x)
(assert-equal 1 test-list-y)
(assert-equal 1 test-list-z)

; list-sum
(assert-equal 6 (list-sum {1 2 3}))
(assert-equal 6 (list-sum {1 2 3 -3 -2 -1 0 6}))

; list-product
(assert-equal 24 (list-product {1 2 3 4}))
(assert-equal 0 (list-product {1 2 3 0 4}))

; list-avg
(assert-equal 2 (list-avg {2 1 3}))
(assert-equal 2 (list-avg {2 1 2 3 2}))
(assert-equal 3 (list-avg {3 3 2 4}))
(assert-equal 0 (list-avg {-1 1 0 0 0 1 -2 1}))
