;
; Assertion framework
;
(fun {assert x message} {
  if (not x) {
    error (join "assert failed:\n\t" message)
  } {}
})

(fun {assert-true x} {
  assert x (join "expected truthy, got: '" (str x) "'")
})

(fun {assert-false x} {
  assert (not x) (join "expected falsy, got: '" (str x) "'")
})

(fun {assert-equal x y} {
  assert (== x y) (join "expected '" (str x) "', got: '" (str y) "'")
})

(fun {assert-unequal x y} {
  assert (!= x y) (join "expected '" (str x) "', got: '" (str y) "'")
})

(fun {assert-nil x} {
  assert (== x nil) (join "expected nil, got: '" (str x) "'")
})

(fun {assert-not-nil x} {
  assert (!= x nil) (join "expected not-nil, got: '" (str x) "'")
})

(fun {assert-length x exp} {
  (assert (== (len x) exp) (join "expected q-expr with length: " (str exp) ", got length: " (str (len x))))
})

(fun {assert-defined x} {
  (assert (isdef x) (join "expected to be defined: " (str x) ", was undefined."))
})

(fun {assert-undefined x} {
  (assert (not (isdef x)) (join "expected to be undefined: " (str x) ", was '" (str x) "'"))
})

(fun {assert-error x} {
  (assert (!= () (try x)) (join "expected to cause an error: " (str x) ", but execution succeeded"))
})

