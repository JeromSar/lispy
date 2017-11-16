(load "std/stdlib.lp")

(moduledef {math add} (\ {x y} {+ x y}))
(moduledef {math sub} (\ {x y} {- x y}))
(modulefun {math mul x y} {* x y})
(modulefun {math div x y} {\ x y})


(module {test} {
  (fun {plus x y} {+ x y})
  (fun {minus x y} {- x y})
  (fun {negative x} {- x})
})
