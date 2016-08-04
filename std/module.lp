; Adds a definition in a module
;
; Usage:
;   (moduledef {math add} (\ {x y} {+ x y}))
;
; Stores as: { {add (\ {x y} {+ x y})} ... }
;

; TODO: Majorly broken -> Fix
(fun {moduledef name f} { do
  ; Validate args
  (if (== (len name) 2) {} {
    error "Function 'moduledef' passed too few arguments!"
  })

  ; Setup vars (as list) - module name, function name
  (= {mname} (head name))
  (= {fname} (tail name))
  
  ; Define the module as an empty list,
  ; if it does not exist already
  ; if (isdef {nil}) {(print "hai")} {(print "hi")}
  (if (isdef mname) {} { do
    (def mname {})
  })
  
  ; New addition:
  ; {add (\ {x y} {+ x y })}
  (def {newdef} (join fname (list f)))
  
  ; Update the module list
  (def mname (cons newdef (eval mname)))
})

; Defines a module.
;
; Usage:
;   (module {math} {
;    (def {plus x y}  {+ x y})
;    (def {minus x y  {- x y}}
;   })
;
; (module {math} { (def {plus x y}  {+ x y}) })
(fun {module mname content} { do
  ; Redefine 'def' to use 'moduledef'
  ; (def {x} 20)
  (= {def} (\ {formals body} {moduledef (join mname formals) body}))
  
  ;(= {def} (\ {_ b} {print "hi"}))
  ;(show def)
  
  (def {plus x y} {+ x y})
  
  (show content)
  
  ; Evaluate body
  (eval content)
})

; Imports all definitions from a module.
;
; Usage:
;   (use {math})
(fun {use mname} {
})

