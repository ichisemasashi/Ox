Lisp > (define x 5)
x
Lisp > (+ (let ((x 3))
     (+ x (* x 10)))
   x)
38
Lisp > (define x 2)
x
Lisp > (let ((x 3)
      (y (+ x 2)))
  (* x y))
15
