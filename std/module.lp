; Adds a definition in a module
;
; Usage:
;   (moduledef {math add} (\ {x y} {+ x y}))
;
; Stores as: { {add (\ {x y} {+ x y})} ... }
;
(fun {moduledef name f} { do
  ; Validate args
  (if (== (len name) 2) {} {
    error "Function 'moduledef' passed too few arguments!"
  })

  (= {mname} (head name))
  (= {fname} (tail name))
  
  ; Define the module as an empty list
  (if (isdef mname) {} { do
    (def mname {})
  })

  ; New definition
  (= {newdef} (join fname (list f)))
  
  ; Redefine the module
  (def mname (cons newdef (eval mname)))
})

; Defines a function in a module
;
; Usage:
;   (modulefun {math mul x y} {* x y})
;
(fun {modulefun desc body} { do
  (= {mod} (head desc))
  (= {name} (head (tail desc)))
  (= {formals} (tail (tail desc)))
  (moduledef (join mod name) (eval (join {\} (list formals) (list body))))
})

; Defines a module.
;
; Usage:
;   (module {math} {
;    (def {plus x y}  {+ x y})
;    (def {minus x y  {- x y}})
;   })
;
; (module {math} { (def {plus x y}  {+ x y}) })
(fun {module mname content} { do

  ; Redefine 'fun' to use 'moduledef'
  (= {fun} (\ {formals body} {modulefun (join mname formals) body}))

  ; Works:
  ;(moduledef {math add} (\ {x y} {+ x y}))
  
  (foreach content (\ {s} { do
    ; Foreach evaluates
  }))
})

; Imports all definitions from a module.
;
; Usage:
;   (use {math})
(fun {use mname} {
})

