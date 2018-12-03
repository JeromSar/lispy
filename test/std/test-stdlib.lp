;
; Lispy standard library tests
;

; debug

(load "std/stdlib.lp")

; Test stdlib packages
(load "test/std/test-atom.lp")
(load "test/std/test-fun.lp")
(load "test/std/test-list.lp")
(load "test/std/test-cond.lp")
(load "test/std/test-math.lp")
(load "test/std/test-map.lp")

(print "All tests OK.")
