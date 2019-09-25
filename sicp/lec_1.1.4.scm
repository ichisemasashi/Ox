Lisp > (define square (lambda (x) (* x x)))
square
Lisp > (square 21)
441
Lisp > (square (+ 2 5))
49
Lisp > (square (square 3))
81
Lisp > (define sum-of-squares (lambda (x y) (+ (square x) (square y))))
sum-of-squares
Lisp > (sum-of-squares 3 4)
25
Lisp > (define f (lambda (a) (sum-of-squares (+ a 1) (* a 2))))
f
Lisp > (f 5)
136
