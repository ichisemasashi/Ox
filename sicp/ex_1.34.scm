Lisp > (define f (lambda (g) (g 2)))
f
Lisp > (define square (lambda (x) (* x x)))
square
Lisp > (f square)
4
Lisp > (f (lambda (z) (* z (+ z 1))))
6
Lisp > (f f) 
Can't apply this CAR.
(2 2)
