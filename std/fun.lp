;
; Function definitions
;

; Define a function
(def {fun} (\ {f b} {
  def (head f) (\ (tail f) b)
}))

; Unpack list for function
(fun {unpack f l} {
  eval (join (list f) l)
})

; Pack list for function
(fun {pack f & xs} {f xs})

; Call function curried or uncurried
(def {curry} unpack)
(def {uncurry} pack)

; Perform several things in sequence
(fun {do & l} {
  if (== l nil)
    {nil}
    {last l}
})

; Open new scope
(fun {let b} {
  ((\ {_} b) ())
})

; Flip operands
(fun {flip f a b} {f b a})

; Ignored
;(fun {ghost & xs} {eval xs})

; Compose a function of two functions
(fun {comp f g x} {f (g x)})
