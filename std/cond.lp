;
; Conditional operations
;
(fun {not x}   {> (- 1 x) 0})
(fun {or x y}  {!= (+ x y) 0})
(fun {and x y} {!= (* x y) 0})

(fun {select & cs} {
  if (== cs nil)
    {error "No selection Found"}
    {if (fst (fst cs)) {snd (fst cs)} {unpack select (tail cs)}}
})

(fun {case x & cs} {
  if (== cs nil)
    ;{error "No case Found"}
    {nil}
    {if (== x (fst (fst cs))) {snd (fst cs)} {
      unpack case (join (list x) (tail cs))}}
})
