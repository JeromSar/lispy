;
; List operations
;

; Make a new map
(fun {map-new & keyvals} {
  if (== vals {{}}) {
    nil
  } {
    
  }
})

; Get the length of a map
;(fun {map-len map} {
;  if (== map nil) {
;    0
;  } {
;    + 1 (map-len (tail map))
;  }
;})
(fun {map-len map} {
  len map
})

; Check if a map has a certain key
(fun {map-has map key} {
  if (== map nil) {
    0
  } {
    if (== (head (fst map)) key) {
      1
    } {
      map-has (tail map) key
    }
  }
})

; Delete all values matching "key" from a map
(fun {map-del map key} {
  if (== map nil) {
    nil
  } {
    if (== (head (fst map)) key) {
      map-del (tail map) key
    } {
      join (head map) (map-del (tail map) key)
    }
  }
})

; Delete all values matching the keys provided
(fun {map-delall map & keys} {
  if (== keys nil) {
    map
  } {
    join (map-del map (head keys)) ((unpack map-delall map) (tail keys))
  }
})

; Put a value in a map, replacing any previous keys
(fun {map-put map key value} {
  join (map-del map key) (list (join key (list value)))
})

; Put all arguments (lists of key-value pairs) in a map, replacing previous values
; TODO Remove previous assignments
(fun {map-putall map & keyvals} {
  join (map) (keyvals)
})

; Get a value by key from a map
(fun {map-get map key} {
  if (== map nil) {
    error "Key not found!"
  } {
    if (== (head (fst map)) key) {
      fst (fst map)
    } {
      map-get (tail map) key
    }
  }
})

; Get all the keys from a map
(fun {map-keys map} {
  if (== map nil) {
    {}
  } {
    join (head (fst map)) (map-keys (tail map))
  }
})

; Get all the values from a map
(fun {map-values map} {
  if (== map nil) {
    {}
  } {
    join (tail (fst map)) (map-keys (tail map))
  }
})

; Get the entry at the nth index
(fun {map-index map index} {
  if (|| (== map nil) (> 0 index)) {
    error "Invalid map index"
  } {
    if (== index 0) {
      fst map
    } {
      map-index (tail map) (- index 1)
    }
  }
})

; Execute a function for each entry
; The function will be passed the key (as a list) as the first argument,
; and the value (evaluated) as the second argument
(fun {map-foreach map f} {
  if (== map nil) {
  } {
    do
      (f (head (fst map)) (snd (fst map)))
      (map-foreach (tail map) f)
  }
})
