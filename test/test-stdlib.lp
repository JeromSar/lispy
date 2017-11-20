;
; Lispy standard library tests
;

(load "std/stdlib.lp")

; Test stdlib packages
(load "test/test-atom.lp")
(load "test/test-fun.lp")
(load "test/test-list.lp")

(print "All tests OK.")
