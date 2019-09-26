Lisp > (define test (lambda (x y) (if (= x 0) 0 y)))
test
Lisp > (define P (lambda () (P)))
P
Lisp > (test 0 P)
0
