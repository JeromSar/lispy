;
; List operations
;

; First, second, or third item in list
(fun {fst l} { eval (head l) })
(fun {snd l} { eval (head (tail l)) })
(fun {trd l} { eval (head (tail (tail l))) })

; Nth item in lsist
(fun {nth n l} {
  if (== n 0)
    {fst l}
    {nth (- n 1) (tail l)}
})

; Last item in List
(fun {last l} {nth (- (len l) 1) l})

; List length - Now included natively
;(fun {len l} {
;  if (== l nil)
;    {0}
;    {+ 1 (len (tail l))}
;})

; Take n items
(fun {take n l} {
  if (== n 0)
    {nil}
    {join (head l) (take (- n 1) (tail l))}
})

; Drop n items
(fun {drop n l} {
  if (== n 0)
    {l}
    {drop (- n 1) (tail l)}
})

; Split at n
(fun {split n l} {list (take n l) (drop n l)})

; Element of list
(fun {elem x l} {
  if (== l nil)
    {false}
    {if (== x (fst l)) {true} {elem x (tail l)}}
})

; Apply function to list
(fun {map f l} {
  if (== l nil)
    {nil}
    {join (list (f (fst l))) (map f (tail l))}
})

; Apply filter to list
(fun {filter f l} {
  if (== l nil)
    {nil}
    {join (if (f (fst l)) {head l} {nil}) (filter f (tail l))}
})

; Fold left
(fun {foldl f z l} {
  if (== l nil)
    {z}
    {foldl f (f z (fst l)) (tail l)}
})

; Fold right
(fun {foldr f z l} {
  if (== l nil)
    {z}
    {foldl f (f z (last l)) (take (- (len l) 1) l)}
})

; For each element in a list
(fun {foreach l f} {
  if (== l nil)
    {nil}
    { 
      do 
      (f (eval (head l)))
      (foreach (tail l) f)
    }
})

;
; List math
;

; Sum and product
(fun {list-sum l} {foldl + 0 l})
(fun {list-product l} {foldl * 1 l})

; Average
(fun {list-avg n} {
  / (list-sum n) (len n)
})
