Lisp > (define A (lambda (x y)
  (cond ((= y 0) 0)
        ((= x 0) (* 2 y))
        ((= y 1) 2)
        (else (A (- x 1) (A x (- y 1)))))))
A
Lisp > (A 1 10)
1024
Lisp > (A 2 4)
65536
Lisp > (A 3 3)
65536
Lisp > (define f (lambda (n) (A 0 n)))
f
Lisp > (f 0)
0
Lisp > (f 1)
2
Lisp > (f 3)
6
Lisp > (define g (lambda (n) (A 1 n)))
g
Lisp > (g 0)
0
Lisp > (g 1)
2
Lisp > (g 2)
4
Lisp > (g 3)
8
Lisp > (define h (lambda (n) (A 2 n)))
h
Lisp > (h 0)
0
Lisp > (h 1)
2
Lisp > (h 2)
4
Lisp > (h 3)
16
Lisp > (define k (lambda (n) (* 5 n n)))
k
Lisp > (k 0)
0
Lisp > (k 1)
5
Lisp > (k 2)
20
Lisp > (k 3)
45
