Lisp > (define abs (lambda (x)
  (cond ((> x 0) x)
        ((= x 0) 0)
        ((< x 0) (- x)))))
abs
Lisp > (abs -1)
1
Lisp > (define abs (lambda (x)
  (cond ((< x 0) (- 0 x))
        (else x))))
abs
Lisp > (abs -5)
5
Lisp > (define abs (lambda (x)
  (if (< x 0)
      (- x)
      x)))
abs
Lisp > (abs 10)
10
Lisp > (abs -30)
30
Lisp > (define and (lambda (x y)
  (cond ((= x true) (cond ((= y true) true)
                           (else false)))
        (else false))))
and
Lisp > (and (> x 5) (< x 10))
FALSE
Lisp > (define or (lambda (x y)
  (cond ((= x true) true)
        ((= y true) true)
        (else false))))
or
Lisp > (define not (lambda (x)
  (cond ((= x true) false)
        (else  true))))
not
Lisp > (define >= (lambda (x y) (or (> x y) (= x y))))
>=
Lisp > (define >= (lambda (x y) (not (< x y))))
>=
Lisp > (>= 5 4)
TRUE
Lisp > (>= 4 5)
FALSE
