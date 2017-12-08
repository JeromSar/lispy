;
; Lispy standard library tests
;

(load "std/stdlib.lp")

; Test stdlib packages
(load "test/test-atom.lp")
(load "test/test-fun.lp")
(load "test/test-list.lp")
(load "test/test-cond.lp")
(load "test/test-math.lp")
(load "test/test-map.lp")

(print "All tests OK.")
