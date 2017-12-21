; not
(assert-true (not false))
(assert-false (not true))

; or
(assert-false (or false false))
(assert-true (or true false))
(assert-true (or false true))
(assert-true (or true true))

; and
(assert-false (and false false))
(assert-false (and true false))
(assert-false (and false true))
(assert-true (and true true))

; select
(assert-equal "hi" (select
    {(== true true) "hi"}
    {(== true true) "hello"}
    {(== true true) "hai"}
    {otherwise "end"}
))
(assert-equal "hello" (select
    {(== true false) "hi"}
    {(== true true) "hello"}
    {(== true true) "hai"}
    {otherwise "end"}
))
(assert-equal "hai" (select
    {(== true false) "hi"}
    {(== true false) "hello"}
    {(== true true) "hai"}
    {otherwise "end"}
))
(assert-equal "end" (select
    {(== true false) "hi"}
    {(== true false) "hello"}
    {(== true false) "hai"}
    {otherwise "end"}
))

; case
(assert-equal "a" (case 0
    {0 "a"}
    {1 "b"}
    {2 "c"}
))
(assert-equal "b" (case 1
    {0 "a"}
    {1 "b"}
    {2 "c"}
))
(assert-equal "c" (case 2
    {0 "a"}
    {1 "b"}
    {2 "c"}
))
(assert-nil (case 3
    {0 "a"}
    {1 "b"}
    {2 "c"}
))
