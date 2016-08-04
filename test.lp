(load "std/stdlib.lp")

(foreach {"1" "2" "3"} (\ {x} {print x}))

(moduledef {math add} (\ {x y} {+ x y}))
(moduledef {math sub} (\ {x y} {- x y}))
(moduledef {math mul} (\ {x y} {* x y}))
(moduledef {math div} (\ {x y} {/ x y}))

(module {test} {
  (def {plus x y} {+ x y})
})
