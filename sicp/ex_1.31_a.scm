Lisp > (define identity (lambda (x) x))
identity
Lisp > (define inc (lambda (n) (+ n 1)))
inc
Lisp > (define product (lambda (term a next b)
  (if (> a b)
    1
    (* (term a)
       (product term (next a) next b)))))
product
Lisp > (define factorial (lambda (n)
  (product identity 1 inc n)))
factorial
Lisp > (factorial 5)
120
Lisp > (factorial 0)
1
Lisp > (define square (lambda (x) (* x x)))
square
Lisp > (define pi-term (lambda (x)
    (/ (* x (+ x 2.0))
       (square (+ x 1)))))
pi-term
Lisp > (define pi-next (lambda (x) (+ x 2)))
pi-next
Lisp > (define pi-prod (lambda (n) (product pi-term 2 pi-next n)))
pi-prod
Lisp > (* 4 (pi-prod 100))
3.157030
Lisp > (* 4 (pi-prod 10000))
3.141979
