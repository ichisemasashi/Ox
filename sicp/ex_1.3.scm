Lisp > (define and (lambda (x y)
  (cond ((= x true) (cond ((= y true) true)
                    (else false)))
        (else false))))
and
Lisp > (define square (lambda (x) (* x x)))
square
Lisp > (define ex-1-3-iter (lambda (a b) (+ (square a) (square b))))
ex-1-3-iter
Lisp > (define ex-1-3 (lambda (x y z)
   (cond ((and (< x y) (< x z)) (ex-1-3-iter y z))
         ((and (< y x) (< y z)) (ex-1-3-iter x z))
         (else (ex-1-3-iter x y)))))
ex-1-3
Lisp > (ex-1-3 1 2 3)
13
Lisp > (ex-1-3 3 2 1)
13
Lisp > (ex-1-3 1 3 2)
13
