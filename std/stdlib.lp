;
; Lispy v0.0.1 standard library
;

; Atoms
(def {nil} {})
(def {true} 1)
(def {false} 0)
(def {otherwise} true)

; Functions
(load "std/fun.lp")

; List
(load "std/list.lp")

; Conditional
(load "std/cond.lp")

; Modules
;(load "std/module.lp")

; Math
(load "std/math.lp")
